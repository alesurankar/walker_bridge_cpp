#pragma once
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <boost/asio.hpp>
#include <array>
#include <thread>


class UdpReceiver : public rclcpp::Node
{
public:
  UdpReceiver();
  ~UdpReceiver();
private:
  void start_receive();
private:
  static constexpr int PORT = 17945;
  boost::asio::io_context io_context_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  std::thread io_thread_;
  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_;
};