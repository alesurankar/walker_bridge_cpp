#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <string>
#include <memory>


class CommandRouter : public rclcpp::Node
{
public:
  CommandRouter();
  ~CommandRouter() override = default;
  void on_udp_message(const std::string& message);
private:
  bool parse_message(const std::string& message);
  void handle_joint_control(const std::string& json);
  void handle_walk_command(const std::string& json);
  void handle_reach_command(const std::string& json);
  void handle_estop_command(const std::string& json);
  void publish_joint_state(const sensor_msgs::msg::JointState& msg);
private:
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
};