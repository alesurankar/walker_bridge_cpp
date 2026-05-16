#include "walker_bridge_cpp/udp_receiver.hpp"
#include "walker_bridge_cpp/command_router.hpp"
#include <rclcpp/rclcpp.hpp>
#include <memory>


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  // 1. Create ROS2 brain node
  auto command_router = std::make_shared<CommandRouter>();

  // 2. Create UDP transport
  UdpReceiver udp(17945);

  // 3. Wire UDP → CommandRouter
  udp.set_callback(
    [command_router](const std::string & msg)
    {
      command_router->on_udp_message(msg);
    }
  );
  udp.start();

  // 4. Run ROS2 spin (main loop)
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(command_router);
  executor.spin();

  // shutdown
  rclcpp::shutdown();

  return 0;
}