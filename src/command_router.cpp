#include "udp_ros_bridge/command_router.hpp"
#include "udp_ros_bridge/protocol/command_decoder.hpp"


using udp_ros_bridge::CommandMessage;

CommandRouter::CommandRouter()
  : 
  Node("command_router")
{
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
    "/joint_states", 10
  );
}

void CommandRouter::on_udp_message(const UdpMessage& msg)
{
  RCLCPP_INFO(this->get_logger(), "Received UDP message");

  auto cmd = udp_ros_bridge::CommandDecoder::decode(
    std::string_view(msg.data, msg.size)
  );

  if (!cmd) {
    RCLCPP_WARN(this->get_logger(), "Failed to decode UDP message");
    return;
  }

  on_command(*cmd);
}

void CommandRouter::on_command(const CommandMessage& cmd)
{
  RCLCPP_INFO(
    this->get_logger(),
    "Routing command type: %s",
    udp_ros_bridge::command_type_to_string(cmd.type)
  );

  using udp_ros_bridge::CommandType;

  switch (cmd.type) {
    case CommandType::BaseVelocity:
      handle_walk_command(cmd);
      break;

    case CommandType::JointPosition:
      handle_joint_control(cmd);
      break;

    case CommandType::CartesianPose:
      handle_reach_command(cmd);
      break;

    case CommandType::Stop:
      handle_estop_command(cmd);
      break;

    default:
      RCLCPP_WARN(this->get_logger(), "Unknown command type");
      break;
  }
}

void CommandRouter::handle_joint_control(const CommandMessage& cmd)
{
  (void)cmd;
  RCLCPP_INFO(this->get_logger(), "Joint control command received");
}

void CommandRouter::handle_walk_command(const CommandMessage& cmd)
{
  if (!std::holds_alternative<udp_ros_bridge::BaseVelocity>(cmd.payload)) {
    RCLCPP_WARN(this->get_logger(), "Invalid payload for BaseVelocity");
    return;
  }

  const auto& vel =
    std::get<udp_ros_bridge::BaseVelocity>(cmd.payload);

  RCLCPP_INFO(
    this->get_logger(),
    "Base velocity command: vx=%.3f vy=%.3f yaw=%.3f",
    vel.vx,
    vel.vy,
    vel.yaw_rate
  );
}

void CommandRouter::handle_reach_command(const CommandMessage& cmd)
{
  (void)cmd;
  RCLCPP_INFO(this->get_logger(), "Reach command received");
}

void CommandRouter::handle_estop_command(const CommandMessage& cmd)
{
  (void)cmd;
  RCLCPP_WARN(this->get_logger(), "EMERGENCY STOP command");
}

void CommandRouter::publish_joint_state(const sensor_msgs::msg::JointState& msg)
{
  RCLCPP_INFO(this->get_logger(), "Publishing JointState");
  joint_pub_->publish(msg);
}

