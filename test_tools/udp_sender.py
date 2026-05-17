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
      "timestamp": int((t - start) * 1000),
      "robot_id": 1,
      "type": "joint_position",
      "priority": 1,
      "payload": {
        "names": [
          "waist_yaw_joint",
          "waist_pitch_joint",
          "head_yaw_joint",
          "head_pitch_joint",
          "L_shoulder_pitch_joint",
          "L_shoulder_roll_joint",
          "L_shoulder_yaw_joint",
          "L_elbow_roll_joint",
          "L_elbow_yaw_joint",
          "L_wrist_pitch_joint",
          "L_wrist_roll_joint",
          "R_shoulder_pitch_joint",
          "R_shoulder_roll_joint",
          "R_shoulder_yaw_joint",
          "R_elbow_roll_joint",
          "R_elbow_yaw_joint",
          "R_wrist_pitch_joint",
          "R_wrist_roll_joint",
          "L_hip_roll_joint",
          "L_hip_yaw_joint",
          "L_hip_pitch_joint",
          "L_knee_pitch_joint",
          "L_ankle_pitch_joint",
          "L_ankle_roll_joint",
          "R_hip_roll_joint",
          "R_hip_yaw_joint",
          "R_hip_pitch_joint",
          "R_knee_pitch_joint",
          "R_ankle_pitch_joint",
          "R_ankle_roll_joint"
        ],
        "positions": [
          0.0,
          0.1,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0
        ]
      }
    }

    data = json.dumps(msg).encode("utf-8")
    sock.sendto(data, (UDP_IP, UDP_PORT))

    print("sent:", vx, yaw_rate)
    time.sleep(0.5)