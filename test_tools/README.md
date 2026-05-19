
```bash
python3 /home/aleur/ros2_ws/src/udp_ros_bridge/test_tools/udp_sender.py

python3 /home/proje/ros2_ws/src/udp_ros_bridge/test_tools/udp_sender.py
```

# UDP Command Protocol
```python
# All messages are sent as UDP JSON packets:
{
  "header": {
    "timestamp": int,
    "robot_id": int,
    "type": "<command_type>",
    "priority": int,
  },
  "payload": { ... }
}
```

# Supported message types
```python

# Base velocity command
{
  "header": {
    "timestamp": int((t - start) * 1000),
    "robot_id": 1,
    "type": "base_velocity",
    "priority": 1,
  },
  "payload": {
    "vx": 0.5,
    "vy": 0.0,
    "yaw_rate": 0.2
  }
}

# Joint position command
{
  "header": {
    "timestamp": int((t - start) * 1000),
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

# Cartesian pose command
{
  "header": {
    "timestamp": 123,
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

# Stop command
{
  "header": {
    "timestamp": 123,
    "robot_id": 1,
    "type": "stop",
    "priority": 1,
  },
  "payload": {}
}
```

# operation goal
```text
Robot control uses “latest message wins” behavior, so only the most recent UDP packet is used and no queueing is required.

- base_velocity (teleop): 30–60 Hz (up to 100 Hz max). Continuous stream while moving; stop triggers slowdown if updates stop.
- cartesian_pose (arm/target): 5–15 Hz while actively adjusting; lower (1–5 Hz) when holding a stable goal.
- stop: event-based only; optional low-rate repetition for safety.

UI runs independently from networking. The robot applies smoothing, interpolation, and safety timeouts (≈200–500 ms) if messages stop arriving.

```