#include "udp_ros_bridge/udp_receiver.hpp"
#include "udp_ros_bridge/command_router.hpp"
#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <chrono>


static constexpr int MAX_MESSAGES_PER_TICK = 20;

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
    size_t processed = 0;

    while (processed < MAX_MESSAGES_PER_TICK)
    {
      if (!udp_.pop_message(msg)) {
        break;
      }

      command_router_->on_udp_message(msg);
      processed++;
    }

    if (processed == MAX_MESSAGES_PER_TICK) {
      budget_limit_hits_++;
    }
  }

  UdpReceiver udp_;
  std::shared_ptr<CommandRouter> command_router_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t budget_limit_hits_ = 0;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<UdpBridgeNode>());
  rclcpp::shutdown();
  return 0;
}