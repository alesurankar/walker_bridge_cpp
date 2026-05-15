#include "walker_bridge_cpp/walker_bridge.hpp"
#include <rclcpp/rclcpp.hpp>


int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<WalkerBridge>();
    node->start_playback();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}