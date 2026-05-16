#include "udp_ros_bridge/command_router.hpp"
#include <nlohmann/json.hpp>
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
  using udp_ros_bridge::CommandType;

  std::cout << "[CommandRouter] Routing command type: "
            << udp_ros_bridge::command_type_to_string(cmd.type)
            << std::endl;

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
      std::cout << "[CommandRouter] Unknown command type\n";
      break;
  }
}

void CommandRouter::handle_joint_control(const CommandMessage& cmd)
{
  (void)cmd;
  std::cout << "[CommandRouter] Joint control command received" << std::endl;
}

void CommandRouter::handle_walk_command(const CommandMessage& cmd)
{
  if (!std::holds_alternative<udp_ros_bridge::BaseVelocity>(cmd.payload)) {
    std::cout << "[CommandRouter] Invalid payload for BaseVelocity\n";
    return;
  }

  const auto& vel =
    std::get<udp_ros_bridge::BaseVelocity>(cmd.payload);

  std::cout << "[CommandRouter] Base velocity command: "
            << vel.vx << ", "
            << vel.vy << ", "
            << vel.yaw_rate << std::endl;
}

void CommandRouter::handle_reach_command(const CommandMessage& cmd)
{
  (void)cmd;
  std::cout << "[CommandRouter] Reach command" << std::endl;
}

void CommandRouter::handle_estop_command(const CommandMessage& cmd)
{
  (void)cmd;
  std::cout << "[CommandRouter] EMERGENCY STOP command" << std::endl;
}

void CommandRouter::publish_joint_state(const sensor_msgs::msg::JointState& msg)
{
  std::cout << "[CommandRouter] Publishing JointState" << std::endl;
  joint_pub_->publish(msg);
}

