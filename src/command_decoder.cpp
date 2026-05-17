#include "udp_ros_bridge/protocol/command_decoder.hpp"
#include <nlohmann/json.hpp>


namespace udp_ros_bridge
{

  std::optional<CommandMessage>CommandDecoder::decode(const std::byte* data, std::size_t size)
  {
    using json = nlohmann::json;

    try {
      std::string_view message(
          reinterpret_cast<const char*>(data),
          size
      );

      json j = json::parse(message);

      CommandMessage cmd;

      cmd.sender_timestamp = j.value<uint64_t>("timestamp", 0);
      cmd.robot_id = j.value("robot_id", "unknown");
      cmd.priority = static_cast<uint8_t>(j.value<int>("priority", 0));

      cmd.type = command_type_from_string(
        j.value("type", "unknown")
      );

      switch (cmd.type)
      {
        case CommandType::BaseVelocity:
        {
          const json params = j.value("payload", json::object());

          BaseVelocity vel;
          vel.vx = params.value("vx", 0.0f);
          vel.vy = params.value("vy", 0.0f);
          vel.yaw_rate = params.value("yaw_rate", 0.0f);

          cmd.payload = vel;
          break;
        }

        default:
          return std::nullopt;
      }

      return cmd;
    }
    catch (...) {
      return std::nullopt;
    }
  }
}