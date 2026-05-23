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
BASE_X = 0.05
BASE_Y = 0.2
BASE_Z = 1.15

while True:
    t = time.monotonic() - start

    # smooth continuous motion (not jerky)
    x = (BASE_X + 0.14 * math.sin(t * 0.6))
    y = (BASE_Y + 0.14 * math.cos(t * 0.4))
    z = (BASE_Z + 0.1 * math.sin(t * 0.3))

    # tiny orientation variation (optional but safe)
    yaw = 0.2 * math.sin(t * 0.5)

    pose_msg = {
        "header": {
            "robot_id": 1,
            "type": "cartesian_pose",
            "priority": 1,
        },
        "payload": {
            "target_link": "L_sixforce_link",
            "frame_id": "base_link",

            #"x": float(x),  # positive front, negative back
            "x": BASE_X,  # positive front, negative back
            # "y": float(y), # positive left, negative right
            "y": BASE_Y,
            #"z": float(z), # positive up, negative down
            "z": BASE_Z,

            # small controlled orientation change
            "qx": -0.05,
            "qy": 0.1,
            # "qz": math.sin(yaw / 2.0),
            "qz": 0.0,
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