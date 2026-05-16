#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <string>
#include <memory>
#include "walker_bridge_cpp/protocol/command_message.hpp"


class CommandRouter : public rclcpp::Node
{
public:
  CommandRouter();
  ~CommandRouter() override = default;
  void on_udp_message(const std::string& message);
private:
  walker_bridge::CommandMessage parse_message(const std::string& message);
  void on_command(const walker_bridge::CommandMessage& cmd);
  void handle_joint_control(const walker_bridge::CommandMessage& cmd);
  void handle_walk_command(const walker_bridge::CommandMessage& cmd);
  void handle_reach_command(const walker_bridge::CommandMessage& cmd);
  void handle_estop_command(const walker_bridge::CommandMessage& cmd);
  void publish_joint_state(const sensor_msgs::msg::JointState& msg);
private:
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
};