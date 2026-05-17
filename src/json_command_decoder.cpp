#include "udp_ros_bridge/decoders/json_command_decoder.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace udp_ros_bridge
{

std::optional<CommandMessage>
JsonCommandDecoder::decode(const std::byte* data, std::size_t size)
{
  if (!data || size == 0)
    return std::nullopt;

  std::string str(reinterpret_cast<const char*>(data), size);

  std::cout << "[JSON DECODER] raw: " << str << std::endl;

  json j;
  try {
    j = json::parse(str);
  } catch (const std::exception& e) {
    std::cout << "[JSON DECODER] parse failed: " << e.what() << std::endl;
    return std::nullopt;
  }

  CommandMessage msg;

  msg.sender_timestamp = j.value("timestamp", 0);
  msg.robot_id = j.value("robot_id", 0);
  msg.priority = j.value("priority", 0);

  std::string type = j.value("type", "unknown");

  std::cout << "[JSON DECODER] type = " << type << std::endl;

  if (type == "base_velocity")
  {
    msg.type = CommandType::BaseVelocity;

    auto& p = msg.payload.emplace<BaseVelocity>();
    p.vx = j["payload"].value("vx", 0.0f);
    p.vy = j["payload"].value("vy", 0.0f);
    p.yaw_rate = j["payload"].value("yaw_rate", 0.0f);
  }
  else if (type == "joint_position")
  {
    msg.type = CommandType::JointPosition;

    auto& p = msg.payload.emplace<JointPosition>();

    auto arr = j["payload"]["data"];
    p.count = std::min<size_t>(arr.size(), 32);

    for (size_t i = 0; i < p.count; i++)
      p.data[i] = arr[i].get<float>();
  }
  else if (type == "cartesian_pose")
  {
    msg.type = CommandType::CartesianPose;

    auto& p = msg.payload.emplace<CartesianPose>();
    auto& pl = j["payload"];

    p.x = pl.value("x", 0.0f);
    p.y = pl.value("y", 0.0f);
    p.z = pl.value("z", 0.0f);
    p.roll = pl.value("roll", 0.0f);
    p.pitch = pl.value("pitch", 0.0f);
    p.yaw = pl.value("yaw", 0.0f);
  }
  else
  {
    msg.type = CommandType::Unknown;
  }

  return msg;
}

}