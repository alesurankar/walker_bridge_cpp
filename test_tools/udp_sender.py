import socket
import json
import time
import random
import os

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(BASE_DIR, "data")
print("DATA_DIR =", DATA_DIR)

def load_poses():
    poses = []
    for file in os.listdir(DATA_DIR):
        if file.endswith(".json"):
            path = os.path.join(DATA_DIR, file)
            with open(path, "r") as f:
                poses.append(json.load(f))
    return poses

poses = load_poses()

if len(poses) == 0:
    raise RuntimeError("No pose files found in data/")

print(f"Loaded {len(poses)} poses")

start = time.monotonic()

while True:
  t = time.monotonic() - start

  # pick random pose
  payload = random.choice(poses)

  pose_msg = {
    "header": {
      "robot_id": 1,
      "type": "cartesian_pose",
      "priority": 1,
    },
    "payload": payload
  }

  sock.sendto(
    json.dumps(pose_msg).encode("utf-8"),
    (UDP_IP, UDP_PORT)
  )

  print(f"Sent pose: x={payload['x']:.3f}, y={payload['y']:.3f}, z={payload['z']:.3f}")

  time.sleep(5)