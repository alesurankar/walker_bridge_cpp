#include "udp_ros_bridge/udp_ros_bridge.hpp"


UdpBridgeNode::UdpBridgeNode()
  : 
  Node("udp_bridge_node"),
  udp_(17945),
  command_router_(std::make_shared<CommandRouter>())
{
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
  rclcpp::Rate rate(10000);

  while (rclcpp::ok() && running_) {
    UdpMessage msg;
    bool processed_any = false;

    // drain everything available
    while (udp_.pop_message(msg)) {
      command_router_->on_udp_message(msg);
      processed_any = true;
    }

    if (!processed_any) {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    // or: rate.sleep(); (optional alternative)
  }
}