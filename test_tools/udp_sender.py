import socket
import json
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

msg = {
    "timestamp": time.time(),
    "robot_id": "humanoid_01",
    "type": "command",
    "command": {
        "mode": "walk",
        "params": {
            "vx": 0.2,
            "vy": 0.0,
            "yaw_rate": 0.1
        }
    },
    "priority": 1
}

while True:
    data = json.dumps(msg).encode("utf-8")
    sock.sendto(data, (UDP_IP, UDP_PORT))
    print("sent")
    time.sleep(0.1)