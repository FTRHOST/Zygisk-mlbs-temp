import socket
import sys
import json
import time

def connect_to_socket():
    # Abstract namespace socket name (must match the C++ definition)
    # Note: In Python, abstract namespace is denoted by a leading null byte.
    SOCKET_NAME = '\0mlbs_ipc'

    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    print(f"Connecting to abstract socket: {repr(SOCKET_NAME)}...")

    try:
        sock.connect(SOCKET_NAME)
        print("Connected successfully! Waiting for data...")
        return sock
    except socket.error as e:
        print(f"Connection failed: {e}")
        print("Make sure the game is running and the mod is injected.")
        return None

def main():
    sock = connect_to_socket()
    if not sock:
        sys.exit(1)

    buffer = ""
    try:
        while True:
            data = sock.recv(4096)
            if not data:
                print("Server disconnected.")
                break

            raw_data = data.decode('utf-8')
            buffer += raw_data

            # Data is newline delimited
            while '\n' in buffer:
                line, buffer = buffer.split('\n', 1)
                if line.strip():
                    try:
                        parsed = json.loads(line)
                        print(f"[Received] {json.dumps(parsed, indent=2)}")
                    except json.JSONDecodeError:
                        print(f"[Raw] {line}")

    except KeyboardInterrupt:
        print("\nStopping...")
    finally:
        sock.close()

if __name__ == "__main__":
    main()
