#include "udp_ros_bridge/decoders/json_command_decoder.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace udp_ros_bridge
{

std::optional<CommandMessage>
JsonCommandDecoder::decode(const std::byte* data, std::size_t size)
{

  std::cout << "\n==============================\n";
  std::cout << "[JSON DECODER] decode() called\n";
  std::cout << "[JSON DECODER] size = " << size << "\n";
  
  if (!data || size == 0) {
    std::cout << "[JSON DECODER] invalid input\n";
    return std::nullopt;
  }

  std::string str(reinterpret_cast<const char*>(data), size);

  std::cout << "[JSON DECODER] raw: " << str << std::endl;

  json j;
  try {
    j = json::parse(str);
  } 
  catch (const std::exception& e) {
    std::cout << "[JSON DECODER] parse failed: " << e.what() << std::endl;
    return std::nullopt;
  }

  std::cout << "[JSON DECODER] parse OK\n";
  CommandMessage msg;

  // ---- timestamp ----
  msg.sender_timestamp = j.value("timestamp", 0);
  std::cout << "[JSON DECODER] timestamp = " << msg.sender_timestamp << "\n";

  // ---- robot_id ----
  msg.robot_id = j.value("robot_id", 0);
  std::cout << "[JSON DECODER] robot_id = " << msg.robot_id << "\n";

  // ---- priority ----
  msg.priority = j.value("priority", 0);
  std::cout << "[JSON DECODER] priority = " << (int)msg.priority << "\n";

  // ---- type ----
  std::string type = j.value("type", "unknown");
  std::cout << "[JSON DECODER] type string = " << type << "\n";

  // ---- mapping ----
  if (type == "base_velocity") {
    msg.type = CommandType::BaseVelocity;
    std::cout << "[JSON DECODER] mapped → BaseVelocity\n";

    auto& p = msg.payload.emplace<BaseVelocity>();
    auto& pl = j["payload"];

    p.vx = pl.value("vx", 0.0f);
    p.vy = pl.value("vy", 0.0f);
    p.yaw_rate = pl.value("yaw_rate", 0.0f);

    std::cout << "[JSON DECODER] vx=" << p.vx
              << " vy=" << p.vy
              << " yaw=" << p.yaw_rate << "\n";
  }
  else if (type == "joint_position") {
    msg.type = CommandType::JointPosition;
    std::cout << "[JSON DECODER] mapped → JointPosition\n";

    auto& p = msg.payload.emplace<JointPosition>();
    auto arr = j["payload"]["data"];

    p.count = std::min<size_t>(arr.size(), 32);

    std::cout << "[JSON DECODER] joint count = " << (int)p.count << "\n";
  }
  else if (type == "cartesian_pose") {
    msg.type = CommandType::CartesianPose;
    std::cout << "[JSON DECODER] mapped → CartesianPose\n";
  }
  else if (type == "stop") {
    msg.type = CommandType::Stop;
    std::cout << "[JSON DECODER] mapped → Stop\n";
  }
  else {
    msg.type = CommandType::Unknown;
    std::cout << "[JSON DECODER] mapped → UNKNOWN\n";
  }

  std::cout << "[JSON DECODER] FINAL enum = " << (int)msg.type << "\n";
  std::cout << "==============================\n";

  return msg;
}

}