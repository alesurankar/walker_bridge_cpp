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

    # --- Cartesian velocity profile ---
    vx = 0.05 * math.sin(t * 0.6)
    vy = 0.04 * math.cos(t * 0.4)
    vz = 0.03 * math.sin(t * 0.3)

    wz = 0.25 * math.sin(t * 0.5)

    msg = {
        "header": {
            "robot_id": 1,
            "type": "cartesian_velocity",
            "priority": 1
        },

        "payload": {
            "frame_id": "base_link",
            "target_link": "L_wrist_roll_link",
            "is_relative": False,

            "vx": float(vx),
            "vy": float(vy),
            "vz": float(vz),

            "wx": 0.0,
            "wy": 0.0,
            "wz": float(wz)
        }
    }

    sock.sendto(
        json.dumps(msg).encode("utf-8"),
        (UDP_IP, UDP_PORT)
    )

    print(
        f"UDP cartesian_velocity "
        f"vx={vx:.3f}, vy={vy:.3f}, vz={vz:.3f}, wz={wz:.3f}"
    )

    time.sleep(SEND_PERIOD)