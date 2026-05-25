import socket
import json
import time
import math

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

start = time.monotonic()
SEND_PERIOD = 0.01  # 100 Hz

while True:
    t = time.monotonic() - start

    # --- base velocity signal ---
    vx = 0.25 * math.sin(t * 0.6)
    vy = 0.20 * math.cos(t * 0.4)
    yaw_rate = 0.35 * math.sin(t * 0.5)

    msg = {
        "header": {
            "robot_id": 1,
            "type": "base_velocity",
            "priority": 1
        },
        "payload": {
            "vx": float(vx),
            "vy": float(vy),
            "yaw_rate": float(yaw_rate)
        }
    }

    sock.sendto(
        json.dumps(msg).encode("utf-8"),
        (UDP_IP, UDP_PORT)
    )

    print(f"UDP vx={vx:.3f}, vy={vy:.3f}, yaw={yaw_rate:.3f}")

    time.sleep(SEND_PERIOD)