#pragma once
#include <string>
#include <cstdint>


namespace udp_ros_bridge
{

struct CartesianPoseCommand
{
  // --- Identity ---
  int robot_id = 0;
  uint64_t timestamp = 0;

  // --- What is being controlled ---
  std::string target_link;   // e.g. "L_wrist", "R_foot", "camera_link"

  // --- Reference frame ---
  std::string frame_id;      // e.g. "world", "base_link", "odom"

  // --- Pose (quaternion, NOT Euler) ---
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  float qx = 0.0f;
  float qy = 0.0f;
  float qz = 0.0f;
  float qw = 1.0f;

  // --- Optional control metadata ---
  float position_gain = 1.0f;
  float orientation_gain = 1.0f;

  bool is_relative = false;   // true = delta pose, false = absolute
};

} // namespace udp_ros_bridge