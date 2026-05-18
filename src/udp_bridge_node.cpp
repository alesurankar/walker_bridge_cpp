#include "udp_ros_bridge/udp_bridge_node.hpp"
#include <chrono>
#include <thread>


UdpBridgeNode::UdpBridgeNode()
  : 
  Node("udp_bridge_node"),
  udp_(17945)
{
  base_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(
    "/walker/base_velocity", 10);
  
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
    "/walker/joint_states", 10);

  pose_pub_ = this->create_publisher<motion_interfaces::msg::CartesianPoseCommand>(
    "/walker/cartesian_pose_command", 10);

  stop_pub_ = this->create_publisher<std_msgs::msg::Empty>(
    "/walker/stop", 10);

  router_.set_joint_callback(
    [this](const udp_ros_bridge::JointPosition& jp) {
      publish_joint_state(jp);
    });

  router_.set_base_callback(
    [this](const udp_ros_bridge::BaseVelocity& bv) {
      publish_base_velocity(bv);
    });

  router_.set_pose_callback(
    [this](const udp_ros_bridge::CartesianPoseCommand& cp) {
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

void UdpBridgeNode::publish_base_velocity(const udp_ros_bridge::BaseVelocity& bv)
{
  RCLCPP_INFO(this->get_logger(),
    "Base velocity: vx=%.2f vy=%.2f yaw=%.2f",
    bv.vx, bv.vy, bv.yaw_rate);
    
  geometry_msgs::msg::Twist msg;

  msg.linear.x = bv.vx;
  msg.linear.y = bv.vy;
  msg.angular.z = bv.yaw_rate;

  base_pub_->publish(msg);
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

void UdpBridgeNode::publish_cartesian_pose(const udp_ros_bridge::CartesianPoseCommand& cp)
{
  RCLCPP_INFO(this->get_logger(),
    "\n[CartesianPoseCommand]\n"
    "  target_link: %s\n"
    "  frame_id: %s\n"
    "  position: (%.2f %.2f %.2f)\n"
    "  orientation (quat): (%.2f %.2f %.2f %.2f)\n"
    "  gains: pos=%.2f ori=%.2f\n"
    "  is_relative: %s",
    cp.target_link.c_str(),
    cp.frame_id.c_str(),
    cp.x, cp.y, cp.z,
    cp.qx, cp.qy, cp.qz, cp.qw,
    cp.position_gain,
    cp.orientation_gain,
    cp.is_relative ? "true" : "false");

  motion_interfaces::msg::CartesianPoseCommand msg;

  msg.header.stamp = this->now();
  msg.header.frame_id = cp.frame_id;

  msg.target_link = cp.target_link;

  msg.pose.position.x = cp.x;
  msg.pose.position.y = cp.y;
  msg.pose.position.z = cp.z;

  msg.pose.orientation.x = cp.qx;
  msg.pose.orientation.y = cp.qy;
  msg.pose.orientation.z = cp.qz;
  msg.pose.orientation.w = cp.qw;

  msg.position_gain = cp.position_gain;
  msg.orientation_gain = cp.orientation_gain;
  msg.is_relative = cp.is_relative;

  pose_pub_->publish(msg);
}

void UdpBridgeNode::publish_stop()
{
  RCLCPP_WARN(this->get_logger(), "STOP command received");

  std_msgs::msg::Empty msg;
  stop_pub_->publish(msg);
}