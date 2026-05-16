#pragma once
#include <boost/asio.hpp>
#include <array>
#include <thread>
#include <functional>
#include <string>
#include <cstdint>
#include <atomic>


class UdpReceiver
{
public:
  using MessageCallback = std::function<void(const std::string& message)>;
public:
  explicit UdpReceiver(uint16_t port);
  ~UdpReceiver();
  void start();
  void stop();
  void set_callback(MessageCallback cb);
private:
  void io_loop();
  void start_receive();
  void handle_receive(const boost::system::error_code& error, std::size_t bytes_received);
private:
  static constexpr std::size_t BUFFER_SIZE = 65535;
  uint16_t port_;
  boost::asio::io_context io_context_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  std::array<char, BUFFER_SIZE> buffer_;
  std::thread io_thread_;
  MessageCallback callback_;
  std::atomic<bool> running_{false};
};