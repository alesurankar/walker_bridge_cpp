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
  auto cmd = udp_ros_bridge::CommandDecoder::decode(
      msg.data,
      msg.size
  );

  if (!cmd) {
    RCLCPP_WARN(this->get_logger(), "Failed to decode UDP message");
    return;
  }

  on_command(*cmd);
}

void CommandRouter::on_command(const CommandMessage& cmd)
{
  // RCLCPP_INFO(
  //   this->get_logger(),
  //   "Routing command type: %s",
  //   udp_ros_bridge::command_type_to_string(cmd.type)
  // );

  using udp_ros_bridge::CommandType;

  switch (cmd.type) {
    case CommandType::BaseVelocity:
      // TODO
      break;

    case CommandType::JointPosition:
      // TODO
      break;

    case CommandType::CartesianPose:
      // TODO
      break;

    case CommandType::Stop:
      // TODO
      break;

    default:
      RCLCPP_WARN(this->get_logger(), "Unknown command type");
      break;
  }
}

void CommandRouter::publish_joint_state(const sensor_msgs::msg::JointState& msg)
{
  RCLCPP_INFO(this->get_logger(), "Publishing JointState");
  joint_pub_->publish(msg);
}

