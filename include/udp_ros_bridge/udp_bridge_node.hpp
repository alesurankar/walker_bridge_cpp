#include "udp_ros_bridge/udp_receiver.hpp"
#include "udp_ros_bridge/command_decoder.hpp"
#include <rclcpp/rclcpp.hpp>
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
private:
  UdpReceiver udp_;
  CommandDecoder command_decoder_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
  std::thread consumer_thread_;
  std::atomic<bool> running_{false};
};