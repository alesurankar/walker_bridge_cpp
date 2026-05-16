#include "walker_bridge_cpp/udp_receiver.hpp"
#include <iostream>


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
}

void UdpReceiver::stop()
{
  if (!running_) {
    return;
  }

  std::cout << "[UdpReceiver] stop()" << std::endl;
  running_ = false;
}

void UdpReceiver::set_callback(MessageCallback cb)
{
  callback_ = cb;
  std::cout << "[UdpReceiver] callback set" << std::endl;
}

void UdpReceiver::io_loop()
{
  std::cout << "[UdpReceiver] io_loop()" << std::endl;
}

void UdpReceiver::start_receive()
{
  std::cout << "[UdpReceiver] start_receive()" << std::endl;
}

void UdpReceiver::handle_receive(const boost::system::error_code& error, std::size_t bytes_received)
{
  (void)error;
  (void)bytes_received;
  std::cout << "[UdpReceiver] handle_receive() bytes: " << bytes_received << std::endl;
}

