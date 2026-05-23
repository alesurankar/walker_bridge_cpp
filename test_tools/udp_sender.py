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
time.sleep(1.5)


# STEP 2: send cartesian target
BASE_X = 0.30
BASE_Y = 0.00
BASE_Z = 0.25

SINE_AMPLITUDE = 0.01
SEND_PERIOD = 2.0  

while True:

  t = time.monotonic()
  elapsed = t - start

  pose_msg = {
    "header": {
      "robot_id": 1,
      "type": "cartesian_pose",
      "priority": 1,
    },
    "payload": {
      "target_link": "L_sixforce_link",
      "frame_id": "base_link",
      "x": 0.25,
      "y": 0.0,
      "z": 0.25,
      "qx": 0.0,
      "qy": 0.0,
      "qz": 0.0,
      "qw": 1.0,
      "position_gain": 0.5,
      "orientation_gain": 0.5,
      "is_relative": False
    }
  }

  print("Sending cartesian pose...")

  sock.sendto(
    json.dumps(pose_msg).encode("utf-8"),
    (UDP_IP, UDP_PORT)
  )

  time.sleep(10)
