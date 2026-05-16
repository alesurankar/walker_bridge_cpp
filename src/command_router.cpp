#include "walker_bridge_cpp/command_router.hpp"
#include <iostream>


using walker_bridge::CommandMessage;

CommandRouter::CommandRouter()
  : 
  Node("command_router")
{
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
    "/joint_states", 10
  );
}

void CommandRouter::on_udp_message(const std::string& message)
{
  std::cout << "[CommandRouter] Received UDP message: " << message << std::endl;

  std::cout << "[CommandRouter] -> parsing" << std::endl;
  CommandMessage cmd = parse_message(message);

  std::cout << "[CommandRouter] -> routing command" << std::endl;
  on_command(cmd);
}

CommandMessage CommandRouter::parse_message(const std::string& message)
{
  (void)message;

  std::cout << "[CommandRouter] Parsing message" << std::endl;

  CommandMessage cmd;
  cmd.type = "command";
  cmd.mode = "walk";
  cmd.vx = 0.2f;
  cmd.vy = 0.0f;
  cmd.yaw_rate = 0.1f;

  return cmd;
}

void CommandRouter::on_command(const CommandMessage& cmd)
{
  std::cout << "[CommandRouter] Routing command mode: " << cmd.mode << std::endl;

  if (cmd.mode == "walk")
    handle_walk_command(cmd);
  else if (cmd.mode == "reach")
    handle_reach_command(cmd);
  else if (cmd.mode == "estop")
    handle_estop_command(cmd);
  else
    std::cout << "[CommandRouter] Unknown mode: " << cmd.mode << std::endl;
}

void CommandRouter::handle_joint_control(const CommandMessage& cmd)
{
  (void)cmd;
  std::cout << "[CommandRouter] Joint control command received" << std::endl;
}

void CommandRouter::handle_walk_command(const CommandMessage& cmd)
{
  std::cout << "[CommandRouter] Walk command: "
            << cmd.vx << ", "
            << cmd.vy << ", "
            << cmd.yaw_rate << std::endl;
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

