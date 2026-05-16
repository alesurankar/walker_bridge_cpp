#include "walker_bridge_cpp/command_router.hpp"
#include <iostream>


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
  (void)message;
  std::cout << "[CommandRouter] UDP message received: " << message << std::endl;
  parse_message(message);
}

bool CommandRouter::parse_message(const std::string& message)
{
  (void)message;
  std::cout << "[CommandRouter] Parsing message" << std::endl;
  return true;
}

void CommandRouter::handle_joint_control(const std::string& json)
{
  (void)json;
  std::cout << "[CommandRouter] Joint control" << std::endl;
}

void CommandRouter::handle_walk_command(const std::string& json)
{
  (void)json;
  std::cout << "[CommandRouter] Walk command" << std::endl;
}

void CommandRouter::handle_reach_command(const std::string& json)
{
  (void)json;
  std::cout << "[CommandRouter] Reach command" << std::endl;
}

void CommandRouter::handle_estop_command(const std::string& json)
{
  (void)json;
  std::cout << "[CommandRouter] ESTOP command" << std::endl;
}

void CommandRouter::publish_joint_state(const sensor_msgs::msg::JointState& msg)
{
  std::cout << "[CommandRouter] Publishing JointState" << std::endl;
  joint_pub_->publish(msg);
}

