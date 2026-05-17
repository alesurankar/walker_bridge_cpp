#include <rclcpp/rclcpp.hpp>
#include "udp_ros_bridge/udp_ros_bridge.hpp"
#include <memory>


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<UdpBridgeNode>();
  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}