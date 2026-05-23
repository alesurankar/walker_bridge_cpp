import socket
import json
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

time.sleep(1.0)  # only sync delay

while True:
    pose_msg = {
        "header": {
            "robot_id": 1,
            "type": "cartesian_pose",
            "priority": 1,
        },
        "payload": {
            "target_link": "R_sixforce_link",
            "frame_id": "base_link",

            "x": 0.1,
            "y": 0.5,
            "z": 0.25,

            "qx": 1.0,
            "qy": 0.0,
            "qz": 0.0,
            "qw": 1.0,

            "position_gain": 0.5,
            "orientation_gain": 0.3,

            "is_relative": False
        }
    }

    sock.sendto(json.dumps(pose_msg).encode("utf-8"), (UDP_IP, UDP_PORT))
    time.sleep(2.0)