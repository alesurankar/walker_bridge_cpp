#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

#include <boost/asio.hpp>
#include <array>
#include <thread>
#include <memory>

class UdpReceiver : public rclcpp::Node
{
public:
  UdpReceiver();
  ~UdpReceiver();

private:
  void start_receive();
  void handle_receive(const boost::system::error_code &error,
                      std::size_t bytes);

private:
  boost::asio::io_context io_context_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;

  std::thread io_thread_;

  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_;
};