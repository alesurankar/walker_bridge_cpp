#pragma once
#include <string>
#include <vector>
#include <variant>
#include <cstdint>


namespace walker_bridge
{
  enum class CommandType
  {
    BaseVelocity,
    JointPosition,
    CartesianPose,
    WholeBody,
    Stop,
    Custom,
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
    uint64_t timestamp = 0;
    std::string robot_id;
    
    CommandType type = CommandType::Unknown;
    uint8_t priority = 0;

    CommandPayload payload;
  };
  
  // -------------------- helpers ------------------------
  inline CommandType command_type_from_string(const std::string& s)
  {
    if (s == "base_velocity") return CommandType::BaseVelocity;
    if (s == "joint_position") return CommandType::JointPosition;
    if (s == "cartesian_pose") return CommandType::CartesianPose;
    if (s == "whole_body") return CommandType::WholeBody;
    if (s == "stop") return CommandType::Stop;
    if (s == "custom") return CommandType::Custom;
    return CommandType::Unknown;
  }

  inline constexpr const char* command_type_to_string(CommandType t)
  {
    switch (t)
    {
      case CommandType::BaseVelocity:  return "base_velocity";
      case CommandType::JointPosition: return "joint_position";
      case CommandType::CartesianPose: return "cartesian_pose";
      case CommandType::WholeBody:     return "whole_body";
      case CommandType::Stop:          return "stop";
      case CommandType::Custom:        return "custom";
      case CommandType::Unknown:       return "unknown";
      default:                         return "unknown";
    }
  }
}