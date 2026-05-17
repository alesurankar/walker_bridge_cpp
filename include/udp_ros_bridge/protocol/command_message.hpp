#pragma once
#include <variant>
#include <cstdint>
#include <vector>
#include <string>


namespace udp_ros_bridge
{
  enum class CommandType : uint8_t
  {
    BaseVelocity,
    JointPosition,
    CartesianPose,
    Stop,
    Unknown
  };

  // -------------------- payload types --------------------
  struct BaseVelocity
  {
    float vx = 0.0f;
    float vy = 0.0f;
    float yaw_rate = 0.0f;
  };
  
  struct JointPosition
  {
    std::vector<std::string> names;
    std::vector<float> positions;
  };

  struct CartesianPose
  {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
  };

  // -------------------- payload variant -----------------
  using CommandPayload = std::variant<
    BaseVelocity,
    JointPosition,
    CartesianPose
  >;

  // -------------------- main message --------------------
  struct CommandMessage
  {
    uint64_t sender_timestamp = 0;
    uint64_t receive_timestamp = 0;
    
    uint32_t robot_id;
    
    CommandType type = CommandType::Unknown;
    uint8_t priority = 0;

    CommandPayload payload;
  };
}