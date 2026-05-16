#include "udp_ros_bridge/protocol/command_decoder.hpp"
#include <nlohmann/json.hpp>


namespace udp_ros_bridge
{

  std::optional<CommandMessage>CommandDecoder::decode(std::string_view message)
  {
    using json = nlohmann::json;

    CommandMessage cmd;

    try {
      json j = json::parse(message);

      cmd.sender_timestamp = j.value<uint64_t>("timestamp", 0);
      cmd.robot_id = j.value("robot_id", "unknown");
      cmd.priority = static_cast<uint8_t>(j.value<int>("priority", 0));

      cmd.type = udp_ros_bridge::command_type_from_string(
        j.value("type", "unknown")
      );

      switch (cmd.type)
      {
        case CommandType::BaseVelocity:
        {
          const json params = j.value("payload", json::object());

          udp_ros_bridge::BaseVelocity vel;
          vel.vx = params.value("vx", 0.0f);
          vel.vy = params.value("vy", 0.0f);
          vel.yaw_rate = params.value("yaw_rate", 0.0f);

          cmd.payload = vel;
          break;
        }

        case CommandType::JointPosition:
        case CommandType::CartesianPose:
        case CommandType::Stop:
        case CommandType::Unknown:
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