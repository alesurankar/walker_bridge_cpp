#include "udp_ros_bridge/udp_receiver.hpp"
#include "udp_ros_bridge/command_router.hpp"
#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <chrono>


class UdpBridgeNode : public rclcpp::Node
{
public:
  UdpBridgeNode()
    : 
    Node("udp_bridge_node"),
    udp_(17945),
    command_router_(std::make_shared<CommandRouter>())
  {
    udp_.start();

    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1),
      std::bind(&UdpBridgeNode::poll_udp, this)
    );
  }
private:
  void poll_udp()
  {
    UdpMessage msg;
    
    while (udp_.pop_message(msg)) {
      std::string data(msg.data, msg.size);
      command_router_->on_udp_message(data);
    }
  }

  UdpReceiver udp_;
  std::shared_ptr<CommandRouter> command_router_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<UdpBridgeNode>());
  rclcpp::shutdown();
  return 0;
}