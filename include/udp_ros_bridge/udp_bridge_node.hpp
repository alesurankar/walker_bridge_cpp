#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>

#include "udp_ros_bridge/udp_receiver.hpp"
#include "udp_ros_bridge/command_router.hpp"
#include "udp_ros_bridge/decoders/json_command_decoder.hpp"
#include "udp_ros_bridge/protocol/command_message.hpp"

#include <memory>
#include <thread>
#include <atomic>


class UdpBridgeNode : public rclcpp::Node
{
public:
  UdpBridgeNode();
  ~UdpBridgeNode();
private:
  void consumer_loop();
  void publish_joint_state(const udp_ros_bridge::JointPosition& jp);
  void publish_base_velocity(const udp_ros_bridge::BaseVelocity& bv);
  void publish_cartesian_pose(const udp_ros_bridge::CartesianPoseCommand& cp);
  void publish_stop();
private:
  UdpReceiver udp_;
  udp_ros_bridge::JsonCommandDecoder decoder_;
  udp_ros_bridge::CommandRouter router_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
  std::thread consumer_thread_;
  std::atomic<bool> running_{false};
  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
};