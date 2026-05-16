import socket
import json
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
start = time.time()

while True:
    msg = {
        "timestamp": int((time.time() - start) * 1000),
        "robot_id": "humanoid_01",
        "type": "base_velocity",
        "priority": 1,

        "payload": {
            "vx": 1.1,
            "vy": 0.0,
            "yaw_rate": 0.1
        }
    }

    data = json.dumps(msg).encode("utf-8")
    sock.sendto(data, (UDP_IP, UDP_PORT))

    print("sent")
    time.sleep(0.5)