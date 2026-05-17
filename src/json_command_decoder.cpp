#include "udp_ros_bridge/decoders/json_command_decoder.hpp"
#include <nlohmann/json.hpp>
#include <iostream>


using json = nlohmann::json;

namespace udp_ros_bridge
{

std::optional<CommandMessage>
JsonCommandDecoder::decode(const std::byte* data, std::size_t size)
{
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
  CommandMessage msg;

  msg.sender_timestamp = j.value("timestamp", 0);
  std::cout << "[JSON DECODER] timestamp = " << msg.sender_timestamp << "\n";
  msg.robot_id = j.value("robot_id", 0);
  std::cout << "[JSON DECODER] robot_id = " << msg.robot_id << "\n";
  msg.priority = j.value("priority", 0);
  std::cout << "[JSON DECODER] priority = " << (int)msg.priority << "\n";
  std::string type = j.value("type", "unknown");
  std::cout << "[JSON DECODER] type string = " << type << "\n";

  // ================= BASE VELOCITY =================
  if (type == "base_velocity") {
    msg.type = CommandType::BaseVelocity;
    auto& p = msg.payload.emplace<BaseVelocity>();
    const auto& pl = j["payload"];

    p.vx = pl.value("vx", 0.0f);
    p.vy = pl.value("vy", 0.0f);
    p.yaw_rate = pl.value("yaw_rate", 0.0f);

    std::cout << "[JSON DECODER] BaseVelocity OK\n";
  }

  // ================= JOINT POSITION =================
  else if (type == "joint_position") {
    msg.type = CommandType::JointPosition;

    auto& p = msg.payload.emplace<JointPosition>();
    auto payload = j["payload"];

    if (!payload.contains("names") || !payload.contains("positions")) {
      std::cout << "[JSON DECODER] ERROR: missing 'names' or 'positions'\n";
      return std::nullopt;
    }
    auto names = payload["names"];
    auto positions = payload["positions"];

    if (!names.is_array() || !positions.is_array()) {
      std::cout << "[JSON DECODER] ERROR: 'names' or 'positions' not arrays\n";
      return std::nullopt;
    }

    size_t n = std::min(names.size(), positions.size());

    p.names.clear();
    p.positions.clear();

    p.names.reserve(n);
    p.positions.reserve(n);

    for (size_t i = 0; i < n; i++) {
      p.names.push_back(names[i].get<std::string>());
      p.positions.push_back(positions[i].get<float>());
    }

    std::cout << "[JSON DECODER] JointPosition OK, count = " << n << "\n";
  }

  // ================= CARTESIAN POSE =================
  else if (type == "cartesian_pose") {
    msg.type = CommandType::CartesianPose;

    const auto& pl = j["payload"];

    udp_ros_bridge::CartesianPoseCommand cp;

    // --- frame + target ---
    cp.target_link = pl.value("target_link", "");
    cp.frame_id    = pl.value("frame_id", "");

    // --- position ---
    cp.x = pl.value("x", 0.0f);
    cp.y = pl.value("y", 0.0f);
    cp.z = pl.value("z", 0.0f);

    // --- orientation ---
    cp.qx = pl.value("qx", 0.0f);
    cp.qy = pl.value("qy", 0.0f);
    cp.qz = pl.value("qz", 0.0f);
    cp.qw = pl.value("qw", 1.0f);

    // --- gains ---
    cp.position_gain    = pl.value("position_gain", 1.0f);
    cp.orientation_gain = pl.value("orientation_gain", 1.0f);

    // --- mode ---
    cp.is_relative = pl.value("is_relative", false);

    msg.payload = std::move(cp);
    std::cout << "[JSON DECODER] mapped → CartesianPose + payload\n";
  }

  // ================= STOP =================
  else if (type == "stop") {
    msg.type = CommandType::Stop;
    std::cout << "[JSON DECODER] mapped → Stop\n";
  }

  // ================= UNKNOWN =================
  else {
    msg.type = CommandType::Unknown;
    std::cout << "[JSON DECODER] mapped → UNKNOWN\n";
  }
  return msg;
}

}