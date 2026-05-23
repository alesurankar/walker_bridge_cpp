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
SEND_PERIOD = 5  # seconds

# motion limits (keep inside reachable workspace)
BASE_X = 0.25
BASE_Y = 0.00
BASE_Z = 0.25

while True:
    t = time.monotonic() - start

    # smooth continuous motion (not jerky)
    x = BASE_X + 0.03 * math.sin(t * 0.6)
    y = BASE_Y + 0.03 * math.cos(t * 0.4)
    z = BASE_Z + 0.01 * math.sin(t * 0.3)

    # tiny orientation variation (optional but safe)
    yaw = 0.2 * math.sin(t * 0.5)

    pose_msg = {
        "header": {
            "robot_id": 1,
            "type": "cartesian_pose",
            "priority": 1,
        },
        "payload": {
            "target_link": "R_sixforce_link",
            "frame_id": "base_link",

            "x": float(x),
            "y": float(y),
            "z": float(z),

            # small controlled orientation change
            "qx": 0.0,
            "qy": 0.0,
            "qz": math.sin(yaw / 2.0),
            "qw": math.cos(yaw / 2.0),

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