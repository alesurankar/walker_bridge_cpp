import socket
import json
import time
import math

UDP_IP = "127.0.0.1"
UDP_PORT = 17945

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

start = time.monotonic()

joint_names = [
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
]

print("Starting fake joint-state stream...")

last_pose_send = 0.0

while True:

    t = time.monotonic()
    timestamp_ms = int((t - start) * 1000)

    #
    # CONTINUOUS JOINT STATE STREAM
    #

    joint_positions = [0.0] * 30

    # slight motion so timestamps + states change
    joint_positions[4] = 0.2 * math.sin(t)

    joint_msg = {
        "header": {
            "timestamp": timestamp_ms,
            "robot_id": 1,
            "type": "joint_position",
            "priority": 1,
        },
        "payload": {
            "names": joint_names,
            "positions": joint_positions
        }
    }

    sock.sendto(
        json.dumps(joint_msg).encode("utf-8"),
        (UDP_IP, UDP_PORT)
    )

    #
    # SEND CARTESIAN TARGET EVERY 5 SECONDS
    #

    if t - last_pose_send > 5.0:

        pose_msg = {
            "header": {
                "timestamp": timestamp_ms,
                "robot_id": 1,
                "type": "cartesian_pose",
                "priority": 1,
            },
            "payload": {
                "target_link": "L_wrist_roll_link",
                "frame_id": "base_link",
                "x": 0.30,
                "y": 0.00,
                "z": 0.20,
                "qx": 0.0,
                "qy": 0.0,
                "qz": 0.0,
                "qw": 1.0,
                "position_gain": 1.0,
                "orientation_gain": 1.0,
                "is_relative": False
            }
        }

        print("Sending cartesian target...")

        sock.sendto(
            json.dumps(pose_msg).encode("utf-8"),
            (UDP_IP, UDP_PORT)
        )

        last_pose_send = t

    #
    # 20 Hz
    #

    time.sleep(0.05)