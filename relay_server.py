import socket
import json
import threading
import time
from flask import Flask, jsonify, request

# --- KONFIGURASI ---
SOCKET_NAME = '\0mlbs_ipc'  # Socket Game (Abstract Namespace)
WEB_PORT = 5000             # Port untuk akses dari PC

app = Flask(__name__)

# Global Store untuk data terakhir dari game
latest_data = {
    "status": "waiting_for_game",
    "last_update": 0,
    "debug": {},
    "room_info": {},
    "ban_pick": {},
    "battle_stats": {}
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
            print("[RELAY] Menunggu Game aktif... (Pastikan Zygisk modul aktif)")
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
                print("[RELAY] Koneksi game putus via Socket (EOF).")
                sock.close()
                time.sleep(1)
                sock = connect_to_game() # Reconnect
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
                    # Update global data
                    latest_data["last_update"] = time.time()
                    latest_data["status"] = "connected"

                    if parsed.get("type") == "heartbeat":
                        # Update all relevant sections if they exist in the payload
                        if "debug" in parsed:
                            latest_data["debug"] = parsed.get("debug")

                        data_payload = parsed.get("data", {})

                        if "room_info" in data_payload:
                            latest_data["room_info"] = data_payload["room_info"]

                        if "ban_pick" in data_payload:
                            latest_data["ban_pick"] = data_payload["ban_pick"]

                        if "battle_stats" in data_payload:
                            latest_data["battle_stats"] = data_payload["battle_stats"]

                    elif parsed.get("type") == "log":
                        print(f"[GAME LOG] {parsed.get('msg')}")

                    # Debug print di terminal HP/Termux
                    state = latest_data.get('debug', {}).get('game_state', 'N/A')
                    print(f"\r[LIVE] State: {state} | Bytes: {len(line)} | BP: {len(latest_data.get('ban_pick', {}))}   ", end="")
                except json.JSONDecodeError:
                    print(f"\n[ERR] Bad JSON: {line[:50]}...")
                except Exception as e:
                    print(f"\n[ERR] Process error: {e}")

        except Exception as e:
            print(f"\n[ERR] Socket Error: {e}")
            time.sleep(1)
            try:
                sock.close()
            except: pass
            sock = connect_to_game()
            game_socket = sock
            byte_buffer = b""

# --- API ENDPOINTS (Diakses dari PC) ---

@app.route('/api/data', methods=['GET'])
def get_data():
    """PC memanggil ini untuk dapat data JSON lengkap"""
    # Hitung latency sederhana
    now = time.time()
    age = now - latest_data["last_update"]
    latest_data["age_seconds"] = age
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

@app.route('/timebattle', methods=['GET'])
def get_timebattle():
    stats = latest_data.get("battle_stats", {})
    return jsonify({
        "time": stats.get("time", 0),
        "lord_countdown": 0, # Placeholder until event logic is fully hooked
        "turtle_countdown": 0
    })

@app.route('/api/control', methods=['POST'])
def control_feature():
    """PC mengirim perintah Start/Stop ke HP"""
    global game_socket
    if not request.is_json:
        return jsonify({"status": "error", "msg": "Body must be JSON"}), 400

    cmd = request.json.get('command')
    if game_socket and cmd in ['start', 'stop']:
        msg = f"cmd:{cmd}"
        try:
            game_socket.sendall(msg.encode())
            return jsonify({"status": "sent", "cmd": cmd})
        except Exception as e:
            return jsonify({"status": "error", "msg": str(e)}), 500

    return jsonify({"status": "invalid_command_or_socket_closed"}), 400

@app.route('/')
def index():
    return """
    <html>
    <head><title>MLBS Relay</title></head>
    <body style="font-family: monospace; background: #222; color: #0f0; padding: 20px;">
        <h1>MLBS Relay Server Running</h1>
        <p>Endpoints:</p>
        <ul>
            <li><a href="/inforoom" style="color: yellow;">/inforoom</a></li>
            <li><a href="/banpick" style="color: yellow;">/banpick</a></li>
            <li><a href="/infobattle" style="color: yellow;">/infobattle</a></li>
            <li><a href="/timebattle" style="color: yellow;">/timebattle</a></li>
            <li><a href="/api/data" style="color: yellow;">/api/data (Full JSON)</a></li>
        </ul>
        <hr>
        <h3>Control:</h3>
        <button onclick="sendCmd('start')">START Feature</button>
        <button onclick="sendCmd('stop')">STOP Feature</button>
        <div id="resp"></div>
        <script>
            function sendCmd(c) {
                fetch('/api/control', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({command: c})
                }).then(r=>r.json()).then(d=>document.getElementById('resp').innerText=JSON.stringify(d));
            }
        </script>
    </body>
    </html>
    """

if __name__ == "__main__":
    print("--- MLBS RELAY SERVER ---")
    print("1. Pastikan script ini berjalan di HP (Termux).")
    print("2. Pastikan Mod Zygisk sudah aktif di game.")

    # Jalankan listener socket di background thread
    t = threading.Thread(target=socket_listener)
    t.daemon = True
    t.start()

    # Jalankan Web Server
    print(f"\n[SERVER] Web API berjalan di port {WEB_PORT}")
    print(f"[SERVER] Akses dari PC: http://[IP_HP_ANDA]:{WEB_PORT}/")

    # Host 0.0.0.0 agar bisa diakses dari perangkat lain di jaringan (WiFi)
    app.run(host='0.0.0.0', port=WEB_PORT, debug=False)
