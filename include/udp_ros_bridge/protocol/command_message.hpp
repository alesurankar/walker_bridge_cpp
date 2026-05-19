#pragma once
#include <motion_interfaces/msg/base_velocity_command.hpp>
#include <motion_interfaces/msg/joint_position_command.hpp>
#include <motion_interfaces/msg/cartesian_pose_command.hpp>
#include <motion_interfaces/msg/stop_command.hpp>

#include <variant>
#include <cstdint>


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

// -------------------- payload variant -----------------
using CommandPayload = std::variant<
    motion_interfaces::msg::BaseVelocityCommand,
    motion_interfaces::msg::JointPositionCommand,
    motion_interfaces::msg::CartesianPoseCommand,
    motion_interfaces::msg::StopCommand
>;

// -------------------- main message --------------------
struct CommandMessage
{
    // transport/runtime metadata
    uint64_t receive_timestamp_ns = 0;

    // decoded header
    uint64_t sender_timestamp = 0;
    uint32_t robot_id = 0;
    uint8_t priority = 0;
    CommandType type = CommandType::Unknown;

    // decoded ROS message payload
    CommandPayload payload;
};

} // namespace udp_ros_bridge