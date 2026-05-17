#include "udp_ros_bridge/command_router.hpp"
#include "udp_ros_bridge/protocol/command_decoder.hpp"


using udp_ros_bridge::CommandMessage;

std::optional<CommandMessage>
CommandDecoder::decode(const UdpMessage& msg)
{
  return udp_ros_bridge::CommandDecoder::decode(
    msg.data,
    msg.size
  );
}