#include "walker_bridge_cpp/udp_receiver.hpp"
#include <rclcpp/rclcpp.hpp>


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<UdpReceiver>();

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}