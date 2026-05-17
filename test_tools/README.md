
```bash
python3 /home/aleur/ros2_ws/src/udp_ros_bridge/test_tools/udp_sender.py
```

# UDP Command Protocol
```python
# All messages are sent as UDP JSON packets:
{
  "timestamp": int,
  "robot_id": int,
  "type": "<command_type>",
  "priority": int,
  "payload": { ... }
}
```

# Supported message types
```python
# Joint position command
{
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

# Base velocity command
{
  "timestamp": int((t - start) * 1000),
  "robot_id": 1,
  "type": "base_velocity",
  "priority": 1,
  "payload": {
    "vx": 0.5,
    "vy": 0.0,
    "yaw_rate": 0.2
  }
}

# Cartesian pose command
{
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

# Stop command
{
  "timestamp": 123,
  "robot_id": 1,
  "type": "stop",
  "priority": 1,
  "payload": {}
}
```