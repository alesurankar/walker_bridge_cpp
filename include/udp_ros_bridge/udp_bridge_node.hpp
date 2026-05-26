#include <rclcpp/rclcpp.hpp>

#include <motion_interfaces/msg/cartesian_pose_command.hpp>
#include <motion_interfaces/msg/stop_command.hpp>
#include "udp_ros_bridge/udp_receiver.hpp"
#include "udp_ros_bridge/command_router.hpp"
#include "udp_ros_bridge/decoders/json_command_decoder.hpp"

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
  void publish_cartesian_pose(const motion_interfaces::msg::CartesianPoseCommand& msg);
  void publish_stop(const motion_interfaces::msg::StopCommand& msg);
private:
  UdpReceiver udp_;
  udp_ros_bridge::JsonCommandDecoder decoder_;
  udp_ros_bridge::CommandRouter router_;
  rclcpp::Publisher<motion_interfaces::msg::CartesianPoseCommand>::SharedPtr pose_pub_;
  rclcpp::Publisher<motion_interfaces::msg::StopCommand>::SharedPtr stop_pub_;
  std::thread consumer_thread_;
  std::atomic<bool> running_{false};
};