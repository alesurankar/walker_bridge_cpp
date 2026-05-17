#include <sensor_msgs/msg/joint_state.hpp>
#include "udp_ros_bridge/udp_bridge_node.hpp"
#include <chrono>
#include <thread>


UdpBridgeNode::UdpBridgeNode()
  : 
  Node("udp_bridge_node"),
  udp_(17945)
{
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
    "/joint_states", 10);

  udp_.start();
  running_ = true;

  consumer_thread_ = std::thread([this]() { consumer_loop(); });
}

UdpBridgeNode::~UdpBridgeNode()
{
  running_ = false;

  if (consumer_thread_.joinable()) {
    consumer_thread_.join();
  }
}

void UdpBridgeNode::consumer_loop()
{
  std::cout << "[CONSUMER] started" << std::endl;
  UdpMessage msg;

  while (rclcpp::ok() && running_) {
    while (udp_.pop_message(msg)) {
      std::cout << "[CONSUMER] GOT MSG size=" << msg.size << std::endl;
      auto cmd_opt = udp_ros_bridge::JsonCommandDecoder::decode(
        msg.data, msg.size);

      if (!cmd_opt) {
        std::cout << "[DECODE] failed" << std::endl;
        continue;
      }
      std::cout << "[DECODE] success" << std::endl;

      const auto& cmd = *cmd_opt;

      switch (cmd.type) {
        case udp_ros_bridge::CommandType::BaseVelocity:
          std::cout << "[DECODE] BaseVelocity" << std::endl;
          // TODO: handle base velocity if needed
          break;

        case udp_ros_bridge::CommandType::JointPosition:
          std::cout << "[DECODE] JointPosition → publish" << std::endl;
          publish_joint_state(cmd);
          break;

        case udp_ros_bridge::CommandType::CartesianPose:
          std::cout << "[DECODE] CartesianPose" << std::endl;
          // TODO: publish pose
          break;

        case udp_ros_bridge::CommandType::Stop:
          std::cout << "[DECODE] Stop command" << std::endl;
          // TODO: stop robot
          break;

        case udp_ros_bridge::CommandType::Unknown:
          std::cout << "[DECODE] WARNING: Unknown command type received" << std::endl;
          break;

        default:
          std::cout << "[DECODE] ERROR: Invalid enum value (corrupted or uninitialized)" << std::endl;
          break;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void UdpBridgeNode::publish_joint_state(const udp_ros_bridge::CommandMessage& cmd)
{
  sensor_msgs::msg::JointState msg;

  const auto& jp =
    std::get<udp_ros_bridge::JointPosition>(cmd.payload);

  msg.position.assign(jp.data, jp.data + jp.count);

  joint_pub_->publish(msg);
}