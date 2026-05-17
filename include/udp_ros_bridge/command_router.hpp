// #pragma once
// #include <rclcpp/rclcpp.hpp>
// #include <sensor_msgs/msg/joint_state.hpp>
// #include <string>
// #include <memory>
// #include "udp_ros_bridge/protocol/command_message.hpp"
// #include "udp_ros_bridge/protocol/udp_message.hpp"


// class CommandRouter : public rclcpp::Node
// {
// public:
//   CommandRouter() = default;
//   ~CommandRouter() override = default;
//   void on_udp_message(const UdpMessage& msg);
// private:
//   void on_command(const udp_ros_bridge::CommandMessage& cmd);
//   void publish_joint_state(const sensor_msgs::msg::JointState& msg);
// private:
//   rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
// };