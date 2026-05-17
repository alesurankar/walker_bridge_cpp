#pragma once
#include <cstddef>
#include <optional>
#include "udp_ros_bridge/protocol/command_message.hpp"

namespace udp_ros_bridge
{
  class CommandDecoder
  {
  public:
    static std::optional<CommandMessage>
    decode(const std::byte* data, std::size_t size);
  };
}