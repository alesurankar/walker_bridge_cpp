#include "udp_ros_bridge/command_decoder.hpp"

using udp_ros_bridge::CommandMessage;

std::optional<CommandMessage>
CommandDecoder::decode(const std::byte* data, std::size_t size)
{
  if (!data || size == 0)
    return std::nullopt;

  CommandMessage cmd;

  // TODO: real decoding logic goes here
  // e.g. memcpy / protobuf / json / custom binary protocol

  return cmd;
}