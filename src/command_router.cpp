#include "udp_ros_bridge/command_router.hpp"
#include <nlohmann/json.hpp>


using walker_bridge::CommandMessage;

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

  CommandMessage cmd = parse_message(
    std::string_view(msg.data, msg.size)
  );

  on_command(cmd);
}

CommandMessage CommandRouter::parse_message(const std::string& message)
{
  using json = nlohmann::json;

  std::cout << "[CommandRouter] Parsing message" << std::endl;

  CommandMessage cmd;

  try {
    json j = json::parse(message);

    cmd.sender_timestamp =
      j.value<uint64_t>("timestamp", 0);

    cmd.robot_id =
      j.value("robot_id", "unknown");

    cmd.priority =
      static_cast<uint8_t>(j.value<int>("priority", 0));

    cmd.type =
      walker_bridge::command_type_from_string(
        j.value("type", "unknown")
      );

    switch (cmd.type) {
      case walker_bridge::CommandType::BaseVelocity:
      {
        const json params = j.value("payload", json::object());

        walker_bridge::BaseVelocity vel;

        vel.vx = params.value("vx", 0.0f);
        vel.vy = params.value("vy", 0.0f);
        vel.yaw_rate = params.value("yaw_rate", 0.0f);

        cmd.payload = vel;
        break;
      }

      default:
        std::cout << "[CommandRouter] Unsupported command type\n";
        break;
    }
  }
  catch (const std::exception& e) {
    std::cout << "[CommandRouter] JSON parse error: "
              << e.what() << std::endl;
  }

  return cmd;
}

void CommandRouter::on_command(const CommandMessage& cmd)
{
  using walker_bridge::CommandType;

  std::cout << "[CommandRouter] Routing command type: "
            << walker_bridge::command_type_to_string(cmd.type)
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
  if (!std::holds_alternative<walker_bridge::BaseVelocity>(cmd.payload)) {
    std::cout << "[CommandRouter] Invalid payload for BaseVelocity\n";
    return;
  }

  const auto& vel =
    std::get<walker_bridge::BaseVelocity>(cmd.payload);

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

