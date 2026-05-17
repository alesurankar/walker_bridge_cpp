#pragma once
#include <optional>
#include "udp_ros_bridge/protocol/udp_message.hpp"
#include "udp_ros_bridge/protocol/command_message.hpp"


class CommandDecoder
{
public:
  CommandDecoder() = default;
  static std::optional<CommandMessage>
  decode(const UdpMessage& msg);
};