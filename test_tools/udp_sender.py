import socket
import json
import time
import math


UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
start = time.monotonic()

# Initial robot state
joint_msg = {
  "header": {
    "timestamp": 0,
    "robot_id": 1,
    "type": "joint_position",
    "priority": 1,
  },
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
      0.0, 0.1, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    ]
  }
}

# STEP 1: establish current robot state
print("Sending initial joint state...")

sock.sendto(
  json.dumps(joint_msg).encode("utf-8"),
  (UDP_IP, UDP_PORT)
)
# Give MoveIt time to receive /joint_states
time.sleep(1.0)


# STEP 2: send cartesian target
while True:

  t = time.monotonic()

  pose_msg = {
    "header": {
      "timestamp": int((t - start) * 1000),
      "robot_id": 1,
      "type": "cartesian_pose",
      "priority": 1,
    },
    "payload": {
      "target_link": "L_wrist_roll_link",
      "frame_id": "base_link",
      "x": 0.3,
      "y": 0.0,
      "z": 1.2,
      "qx": 0.0,
      "qy": 0.0,
      "qz": 0.0,
      "qw": 1.0,
      "position_gain": 1.0,
      "orientation_gain": 1.0,
      "is_relative": False
    }
  }

  print("Sending cartesian pose...")

  sock.sendto(
    json.dumps(pose_msg).encode("utf-8"),
    (UDP_IP, UDP_PORT)
  )

  time.sleep(10)
