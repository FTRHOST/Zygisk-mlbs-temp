import socket
import json
import threading
import time
from flask import Flask, jsonify, request
from flask_socketio import SocketIO

# --- KONFIGURASI ---
SOCKET_NAME = '\x00mlbs_ipc'  # Socket Game (Abstract Namespace)
WEB_PORT = 5000             # Port untuk akses dari PC

app = Flask(__name__)
# Inisialisasi SocketIO dengan CORS allow all
socketio = SocketIO(app, cors_allowed_origins="*")

# Global Store untuk data terakhir dari game
latest_data = {
    "status": "waiting_for_game",
    "last_update": 0,
    "debug": {},
    "room_info": {},
    "ban_pick": {},
    "battle_stats": {},
    "meta": {},
    "result": {},
    "events": []
}
game_socket = None

def connect_to_game():
    """Mencoba terhubung ke Unix Domain Socket game (Abstract Namespace)"""
    global game_socket
    while True:
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(SOCKET_NAME)
            print("[RELAY] Terhubung ke Game Process!")
            return sock
        except Exception:
            # print("[RELAY] Menunggu Game aktif...")
            time.sleep(2)

def socket_listener():
    """Thread khusus untuk mendengar data RAW dari C++"""
    global latest_data, game_socket

    sock = connect_to_game()
    game_socket = sock
    byte_buffer = b""

    while True:
        try:
            data = sock.recv(4096)
            if not data:
                print("[RELAY] Koneksi game putus.")
                sock.close()
                time.sleep(1)
                sock = connect_to_game()
                game_socket = sock
                byte_buffer = b""
                continue

            byte_buffer += data

            while b'\n' in byte_buffer:
                line_bytes, byte_buffer = byte_buffer.split(b'\n', 1)
                line = line_bytes.decode('utf-8', errors='ignore').strip()
                if not line: continue

                try:
                    parsed = json.loads(line)
                    latest_data["last_update"] = time.time()
                    
                    if latest_data["status"] == "waiting_for_game":
                        latest_data["status"] = "connected"

                    # 1. HANDLE HEARTBEAT (Update Stats)
                    if parsed.get("type") == "heartbeat":
                        if "debug" in parsed:
                            latest_data["debug"] = parsed.get("debug")

                        data_payload = parsed.get("data", {})
                        
                        if "room_info" in data_payload:
                            latest_data["room_info"] = data_payload["room_info"]
                        if "battle_stats" in data_payload:
                            latest_data["battle_stats"] = data_payload["battle_stats"]
                        if "meta" in data_payload:
                            latest_data["meta"] = data_payload["meta"]

                        # HANDLE RESULT MVP & GAME OVER
                        if "result" in data_payload:
                            res = data_payload["result"]
                            latest_data["result"] = res
                            
                            is_finished_now = (str(res.get("is_finished")).lower() == "true")
                            was_finished_before = (latest_data.get("status") == "game_over")

                            if is_finished_now and not was_finished_before:
                                latest_data["status"] = "game_over"
                                event_obj = {
                                    "type": "game_over",
                                    "win_camp": res.get("win_camp"),
                                    "mvp_uids": [res.get("mvp_win_uid"), res.get("mvp_lose_uid")]
                                }
                                latest_data["events"].append(event_obj)
                                socketio.emit('game_event', event_obj) # Push event via WS
                                print("\n[GAME] GAME OVER DETECTED!")

                    # 2. HANDLE BAN/PICK UPDATE (HOOK)
                    elif parsed.get("type") == "ban_pick_update":
                        bp_data = parsed.get("data")
                        if not isinstance(latest_data["ban_pick"], dict):
                            latest_data["ban_pick"] = {}
                        
                        pos_key = str(bp_data.get("pos"))
                        latest_data["ban_pick"][pos_key] = {
                            "hero": bp_data.get("hero_id"),
                            "confirmed": bp_data.get("is_confirmed"),
                            "state": bp_data.get("state")
                        }
                        
                        if str(bp_data.get("is_confirmed")).lower() == "true":
                             event_obj = {
                                "type": "pick_lock",
                                "msg": f"Hero {bp_data.get('hero_id')} LOCKED at Pos {bp_data.get('pos')}",
                                "data": bp_data
                             }
                             latest_data["events"].append(event_obj)
                             socketio.emit('game_event', event_obj)

                    # 3. HANDLE GENERIC EVENTS (e.g. First Blood)
                    elif parsed.get("type") == "event":
                        event_data = parsed.get("data", {})
                        latest_data["events"].append(event_data)
                        socketio.emit('game_event', event_data)
                        print(f"\n[EVENT] {event_data.get('msg')}")

                    elif parsed.get("type") == "log":
                        print(f"[GAME LOG] {parsed.get('msg')}")

                    # BROADCAST SEMUA DATA TERBARU VIA WS
                    socketio.emit('game_data', latest_data)

                    # Limit events history
                    if len(latest_data["events"]) > 100:
                        latest_data["events"].pop(0)

                    # Debug output
                    state = latest_data.get('debug', {}).get('game_state', 'N/A')
                    print(f"\r[LIVE] State: {state} | Update: {len(line)} bytes   ", end="")
                    
                except json.JSONDecodeError:
                    print(f"\n[ERR] Bad JSON: {line[:50]}...")
                except Exception as e:
                    print(f"\n[ERR] Process error: {e}")

        except Exception as e:
            print(f"\n[ERR] Socket Error: {e}")
            time.sleep(1)
            try: sock.close()
            except: pass
            sock = connect_to_game()
            game_socket = sock
            byte_buffer = b""

# --- API ENDPOINTS ---

@app.route('/api/data', methods=['GET'])
def get_data():
    return jsonify(latest_data)

@app.route('/inforoom', methods=['GET'])
def get_inforoom():
    return jsonify(latest_data.get("room_info", {}))

@app.route('/banpick', methods=['GET'])
def get_banpick():
    return jsonify(latest_data.get("ban_pick", {}))

@app.route('/infobattle', methods=['GET'])
def get_infobattle():
    return jsonify(latest_data.get("battle_stats", {}))

@app.route('/api/control', methods=['POST'])
def control_feature():
    global game_socket
    if not request.is_json:
        return jsonify({"status": "error", "msg": "Body must be JSON"}), 400
    cmd = request.json.get('command')
    if game_socket and cmd in ['start', 'stop']:
        try:
            game_socket.sendall(f"cmd:{cmd}".encode())
            return jsonify({"status": "sent", "cmd": cmd})
        except Exception as e:
            return jsonify({"status": "error", "msg": str(e)}), 500
    return jsonify({"status": "invalid_command_or_socket_closed"}), 400

@app.route('/')
def index():
    return f"""
    <html>
    <head><title>MLBS Relay WS</title></head>
    <body style="font-family: monospace; background: #222; color: #0f0; padding: 20px;">
        <h1>MLBS Relay Server (WebSocket Active)</h1>
        <p>Status: <b>{latest_data['status']}</b></p>
        <p>WebSocket Event: <b>'game_data'</b> (Full State), <b>'game_event'</b> (Instant Event)</p>
        <hr>
        <ul>
            <li><a href="/api/data" style="color: yellow;">/api/data (JSON)</a></li>
            <li><a href="/inforoom" style="color: yellow;">/inforoom</a></li>
            <li><a href="/banpick" style="color: yellow;">/banpick</a></li>
            <li><a href="/infobattle" style="color: yellow;">/infobattle</a></li>
        </ul>
    </body>
    </html>
    """

if __name__ == "__main__":
    t = threading.Thread(target=socket_listener)
    t.daemon = True
    t.start()
    print(f"[SERVER] Web Socket & API berjalan di port {WEB_PORT}")
    socketio.run(app, host='0.0.0.0', port=WEB_PORT, debug=False, allow_unsafe_werkzeug=True)