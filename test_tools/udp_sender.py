import socket
import json
import time
import math


UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
start = time.monotonic()

while True:
    t = time.monotonic()
    vx = 1.1 + 0.5 * math.sin(t)
    yaw_rate = 0.2 * math.cos(t)

    msg = {
      "timestamp": 123,
      "robot_id": 1,
      "type": "cartesian_pose",
      "priority": 1,
      "payload": {
        "x": 0.3,
        "y": 0.0,
        "z": 1.2,
        "roll": 0.0,
        "pitch": 0.5,
        "yaw": 0.0
      }
    }

    data = json.dumps(msg).encode("utf-8")
    sock.sendto(data, (UDP_IP, UDP_PORT))

    print("sent:", vx, yaw_rate)
    time.sleep(0.5)