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

  router_.set_joint_callback(
    [this](const udp_ros_bridge::JointPosition& jp) {
      publish_joint_state(jp);
    });

  router_.set_base_callback(
    [this](const udp_ros_bridge::BaseVelocity& bv) {
      publish_base_velocity(bv);
    });

  router_.set_pose_callback(
    [this](const udp_ros_bridge::CartesianPose& cp) {
      publish_cartesian_pose(cp);
    });

  router_.set_stop_callback(
    [this]() {
      publish_stop();
    });

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
  UdpMessage msg;

  while (rclcpp::ok() && running_) {
    while (udp_.pop_message(msg)) {
      auto cmd_opt = decoder_.decode(msg.data, msg.size);

      if (!cmd_opt) {
        continue;
      }
      router_.route(*cmd_opt);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void UdpBridgeNode::publish_joint_state(const udp_ros_bridge::JointPosition& jp)
{
  sensor_msgs::msg::JointState msg;

  msg.header.stamp = this->now();
  msg.name = jp.names;
  msg.position.resize(jp.positions.size());

  for (size_t i = 0; i < jp.positions.size(); i++) {
    msg.position[i] = static_cast<double>(jp.positions[i]);
  }

  RCLCPP_INFO(this->get_logger(),
    "Publishing JointState: %zu joints",
    msg.name.size());

  joint_pub_->publish(msg);
}

void UdpBridgeNode::publish_base_velocity(const udp_ros_bridge::BaseVelocity& bv)
{
  RCLCPP_INFO(this->get_logger(),
    "Base velocity: vx=%.2f vy=%.2f yaw=%.2f",
    bv.vx, bv.vy, bv.yaw_rate);
  // publish to Twist or custom topic later
}

void UdpBridgeNode::publish_cartesian_pose(const udp_ros_bridge::CartesianPose& cp)
{
  RCLCPP_INFO(this->get_logger(),
    "Cartesian pose: x=%.2f y=%.2f z=%.2f r=%.2f p=%.2f y=%.2f",
    cp.x, cp.y, cp.z,
    cp.roll, cp.pitch, cp.yaw);
  // later: geometry_msgs::msg::PoseStamped
}

void UdpBridgeNode::publish_stop()
{
  RCLCPP_WARN(this->get_logger(), "STOP command received");
}