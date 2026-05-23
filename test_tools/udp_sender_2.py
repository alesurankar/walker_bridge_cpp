import socket
import json
import time
import math
import random

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

start = time.monotonic()

# slower command rate (important for MoveIt stability)
SEND_PERIOD = 0.5  # seconds

# motion limits (keep inside reachable workspace)
x = -0.3 # positive front, negative back
y = -0.20  # positive left, negative right
z = 0.4 # positive up, negative down  (1.15 max) (0.5 višina ramen)

# -1.0 / 1.0
qx = 0.0
qy = 0.0
qz = 0.2
qw = 0.1

while True:
    t = time.monotonic() - start
    pose_msg = {
        "header": {
            "robot_id": 1,
            "type": "cartesian_pose",
            "priority": 1,
        },
        "payload": {
            "target_link": "R_sixforce_link",
            "frame_id": "base_link",

            "x": x,  
            "y": y,
            "z": z,

            # small controlled orientation change
            "qx": qx,
            "qy": qy,
            "qz": qz,
            "qw": qw,

            "position_gain": 0.4,
            "orientation_gain": 0.2,

            "is_relative": False
        }
    }

    sock.sendto(
        json.dumps(pose_msg).encode("utf-8"),
        (UDP_IP, UDP_PORT)
    )

    print(f"Sent pose: x={x:.3f}, y={y:.3f}, z={z:.3f}")

    time.sleep(SEND_PERIOD)