#pragma once
#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <array>
#include <thread>
#include <string>
#include <cstdint>
#include <atomic>
#include "udp_ros_bridge/protocol/udp_message.hpp"


class UdpReceiver
{
public:
  explicit UdpReceiver(uint16_t port);
  ~UdpReceiver();
  void start();
  void stop();
  bool pop_message(UdpMessage& out_msg);
private:
  void start_receive();
  void handle_receive(const boost::system::error_code& error, std::size_t bytes_received);
private:
  static constexpr std::size_t BUFFER_SIZE = 1400;
  uint16_t port_;
  boost::asio::io_context io_context_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  std::array<char, BUFFER_SIZE> buffer_;
  std::thread io_thread_;
  boost::lockfree::queue<UdpMessage, boost::lockfree::capacity<2048>> queue_;
  std::atomic<bool> running_{false};
};