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
  while (rclcpp::ok() && running_)
  {
    UdpMessage msg;

    while (udp_.pop_message(msg))
    {
      // STEP 1: decode only
      auto cmd_opt = command_decoder_.decode(
        msg.data,
        msg.size
      );

      if (!cmd_opt) {
        continue;
      }

      const auto& cmd = *cmd_opt;

      // STEP 2: route in NODE (ROS boundary)
      switch (cmd.type)
      {
        case udp_ros_bridge::CommandType::JointPosition:
        {
          publish_joint_state(cmd);
          break;
        }

        case udp_ros_bridge::CommandType::BaseVelocity:
        {
          // publish cmd_vel here
          break;
        }

        case udp_ros_bridge::CommandType::Stop:
        {
          // emergency stop handling
          break;
        }

        default:
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