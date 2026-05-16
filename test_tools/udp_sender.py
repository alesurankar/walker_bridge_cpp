import socket
import json
import time
import math


UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
start = time.time()

while True:
    t = time.time()
    vx = 1.1 + 0.5 * math.sin(t)
    yaw_rate = 0.2 * math.cos(t)

    msg = {
        "timestamp": int((t - start) * 1000),
        "robot_id": "humanoid_01",
        "type": "base_velocity",
        "priority": 1,
        "payload": {
            "vx": vx,
            "vy": 0.0,
            "yaw_rate": yaw_rate
        }
    }

    data = json.dumps(msg).encode("utf-8")
    sock.sendto(data, (UDP_IP, UDP_PORT))

    print("sent:", vx, yaw_rate)
    time.sleep(0.5)