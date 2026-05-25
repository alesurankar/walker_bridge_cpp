#include "udp_ros_bridge/udp_bridge_node.hpp"
#include <chrono>
#include <thread>


UdpBridgeNode::UdpBridgeNode()
  : 
  Node("udp_bridge_node"),
  udp_(17945)
{
  base_pub_ = this->create_publisher<motion_interfaces::msg::BaseVelocityCommand>(
    "/walker/base_velocity_command", 10);
  
  joint_pub_ = this->create_publisher<motion_interfaces::msg::JointPositionCommand>(
    "/walker/joint_position_command", 10);

  velocity_pub_ = this->create_publisher<motion_interfaces::msg::CartesianVelocityCommand>(
    "/walker/cartesian_velocity_command", 10);

  pose_pub_ = this->create_publisher<motion_interfaces::msg::CartesianPoseCommand>(
    "/walker/cartesian_pose_command", 10);

  stop_pub_ = this->create_publisher<motion_interfaces::msg::StopCommand>(
    "/walker/stop_command", 10);

  router_.set_base_callback(
    [this](const motion_interfaces::msg::BaseVelocityCommand& msg) {
      publish_base_velocity(msg);
    });

  router_.set_joint_callback(
    [this](const motion_interfaces::msg::JointPositionCommand& msg) {
      publish_joint_state(msg);
    });

  router_.set_velocity_callback(
    [this](const motion_interfaces::msg::CartesianVelocityCommand& msg) {
      publish_cartesian_velocity(msg);
    });

  router_.set_pose_callback(
    [this](const motion_interfaces::msg::CartesianPoseCommand& msg) {
      publish_cartesian_pose(msg);
    });

  router_.set_stop_callback(
    [this](const motion_interfaces::msg::StopCommand& msg) {
      publish_stop(msg);
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
  UdpMessage latest_msg;

  while (rclcpp::ok() && running_) {
    bool got_message = false;

    while (udp_.pop_message(msg)) {
      latest_msg = msg;
      got_message = true;
    }

    if (got_message) {
      auto cmd_opt = decoder_.decode(latest_msg.data, latest_msg.size);

      if (cmd_opt) {
        router_.route(*cmd_opt);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}

void UdpBridgeNode::publish_base_velocity(const motion_interfaces::msg::BaseVelocityCommand& msg)
{
  auto stamped_msg = msg;
  stamped_msg.metadata.stamp = this->get_clock()->now();

  RCLCPP_INFO(
    get_logger(),
    "BASE | ros_time=%.9f",
    rclcpp::Time(stamped_msg.metadata.stamp).seconds());

  base_pub_->publish(stamped_msg);
}

void UdpBridgeNode::publish_joint_state(const motion_interfaces::msg::JointPositionCommand& msg)
{
  RCLCPP_INFO(this->get_logger(),
    "Publishing JointPositionCommand");

  auto stamped_msg = msg;
  stamped_msg.metadata.stamp = this->get_clock()->now();

  RCLCPP_INFO(
    get_logger(),
    "JOINT | ros_time=%.9f",
    rclcpp::Time(stamped_msg.metadata.stamp).seconds());
  joint_pub_->publish(stamped_msg);
}

void UdpBridgeNode::publish_cartesian_velocity(const motion_interfaces::msg::CartesianVelocityCommand& msg)
{
  RCLCPP_INFO(this->get_logger(),
    "Publishing CartesianVelocityCommand");

  auto stamped_msg = msg;
  stamped_msg.metadata.stamp = this->get_clock()->now();

  RCLCPP_INFO(
    get_logger(),
    "VELOCITY | ros_time=%.9f",
    rclcpp::Time(stamped_msg.metadata.stamp).seconds());
  velocity_pub_->publish(stamped_msg);
}

void UdpBridgeNode::publish_cartesian_pose(const motion_interfaces::msg::CartesianPoseCommand& msg)
{
  RCLCPP_INFO(this->get_logger(),
    "Publishing CartesianPoseCommand");

  auto stamped_msg = msg;
  stamped_msg.metadata.stamp = this->get_clock()->now();

  RCLCPP_INFO(
    get_logger(),
    "POSE | ros_time=%.9f",
    rclcpp::Time(stamped_msg.metadata.stamp).seconds());
  pose_pub_->publish(stamped_msg);
}

void UdpBridgeNode::publish_stop(const motion_interfaces::msg::StopCommand& msg)
{
  RCLCPP_WARN(this->get_logger(),
    "Publishing StopCommand");

  auto stamped_msg = msg;
  stamped_msg.metadata.stamp = this->get_clock()->now();

  RCLCPP_INFO(
    get_logger(),
    "STOP | ros_time=%.9f",
    rclcpp::Time(stamped_msg.metadata.stamp).seconds());
  stop_pub_->publish(stamped_msg);
}