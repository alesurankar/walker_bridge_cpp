#include "udp_ros_bridge9/udp_receiver.hpp"
#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>


using udp = boost::asio::ip::udp;

UdpReceiver::UdpReceiver(uint16_t port)
  : 
  port_(port),
  io_context_(),
  work_guard_(boost::asio::make_work_guard(io_context_)),
  socket_(io_context_)
{
  std::cout << "[UdpReceiver] constructed on port " << port_ << std::endl;
}

UdpReceiver::~UdpReceiver()
{
  stop();
  std::cout << "[UdpReceiver] destroyed" << std::endl;
}

void UdpReceiver::start()
{
  std::cout << "[UdpReceiver] start()" << std::endl;
  running_ = true;

  if (!socket_.is_open()) {
    socket_.open(udp::v4());
  }
  
  boost::asio::socket_base::reuse_address option(true);
  socket_.set_option(option);

  socket_.bind(udp::endpoint(
    udp::v4(),
    port_
  ));

  start_receive();
  io_thread_ = std::thread([this]() {
    io_context_.run();
  });

}

void UdpReceiver::stop()
{
  if (!running_) {
    return;
  }

  std::cout << "[UdpReceiver] stop()" << std::endl;
  running_ = false;

  boost::system::error_code ec;
  socket_.cancel(ec);
  socket_.close(ec);
  io_context_.stop();

  if (io_thread_.joinable()) {
    io_thread_.join();
  }
}

bool UdpReceiver::pop_message(UdpMessage& out_msg)
{
  return queue_.pop(out_msg);
}

void UdpReceiver::start_receive()
{
  std::cout << "=============================" << std::endl;
  std::cout << "[UdpReceiver] start_receive()" << std::endl;
  socket_.async_receive_from(
    boost::asio::buffer(buffer_),
    remote_endpoint_,
    [this](const boost::system::error_code& error, std::size_t bytes_received)
    {
      handle_receive(error, bytes_received);

      if (running_) {
        start_receive();
      }
    }
  );
}

void UdpReceiver::handle_receive(const boost::system::error_code& error, std::size_t bytes_received)
{
  if (error == boost::asio::error::operation_aborted) {
    return;
  }

  if (error) {
    std::cerr << "[UdpReceiver] error: " << error.message() << std::endl;
    return;
  }

  UdpMessage msg;
  auto now = std::chrono::system_clock::now();
  msg.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
    now.time_since_epoch()
  ).count();

  if (bytes_received > BUFFER_SIZE) {
    std::cerr << "[UdpReceiver] UDP packet truncated\n";
  }
  msg.size = static_cast<uint16_t>(
    std::min<std::size_t>(bytes_received, BUFFER_SIZE)
  );

  std::memcpy(msg.data, buffer_.data(), msg.size);

  if (!queue_.push(msg)) {
    std::cerr << "[UdpReceiver] queue full, dropping message\n";
  }
}