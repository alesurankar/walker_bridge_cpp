#include "udp_ros_bridge/decoders/json_command_decoder.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <algorithm>


using json = nlohmann::json;

namespace udp_ros_bridge
{

std::optional<CommandMessage>
JsonCommandDecoder::decode(const std::byte* data, std::size_t size)
{
  // =====================================================
  // VALIDATE INPUT
  // =====================================================
  if (!data || size == 0) {

    std::cout
      << "[JSON DECODER] invalid input\n";

    return std::nullopt;
  }

  // =====================================================
  // RAW STRING
  // =====================================================
  std::string str(
    reinterpret_cast<const char*>(data),
    size);

  std::cout
    << "[JSON DECODER] raw: "
    << str
    << std::endl;

  // =====================================================
  // PARSE JSON
  // =====================================================
  json j;

  try {
    j = json::parse(str);
  }
  catch (const std::exception& e) {

    std::cout
      << "[JSON DECODER] parse failed: "
      << e.what()
      << std::endl;

    return std::nullopt;
  }

  // =====================================================
  // VALIDATE ROOT
  // =====================================================
  if (!j.contains("header")) {

    std::cout
      << "[JSON DECODER] missing header\n";

    return std::nullopt;
  }

  if (!j.contains("payload")) {
    std::cout
      << "[JSON DECODER] missing payload\n";

    return std::nullopt;
  }

  const auto& header =
    j["header"];
  const auto& payload =
    j["payload"];

  // =====================================================
  // CREATE COMMAND MESSAGE
  // =====================================================
  CommandMessage msg;

  // =====================================================
  // HEADER
  // =====================================================
  msg.robot_id =
    header.value("robot_id", 0);
  msg.priority =
    header.value("priority", 0);
  const std::string type =
    header.value("type", "unknown");
  std::cout
    << "[JSON DECODER] type = "
    << type
    << std::endl;


  // ================= CARTESIAN POSE =================
  if (type == "cartesian_pose") {
    msg.type = CommandType::CartesianPose;

    auto& cp =
      msg.payload.emplace<
        motion_interfaces::msg::CartesianPoseCommand>();

    // basic fields
    cp.target_link =
      payload.value("target_link", "");

    cp.frame_id =
      payload.value("frame_id", "");

    cp.is_relative =
      payload.value("is_relative", false);

    // position
    cp.pose.position.x =
      payload.value("x", 0.0);

    cp.pose.position.y =
      payload.value("y", 0.0);

    cp.pose.position.z =
      payload.value("z", 0.0);

    // orientation
    cp.pose.orientation.x =
      payload.value("qx", 0.0);

    cp.pose.orientation.y =
      payload.value("qy", 0.0);

    cp.pose.orientation.z =
      payload.value("qz", 0.0);

    cp.pose.orientation.w =
      payload.value("qw", 1.0);

    // gains

    cp.position_gain =
      payload.value("position_gain", 1.0);

    cp.orientation_gain =
      payload.value("orientation_gain", 1.0);

    std::cout
      << "[JSON DECODER] CartesianPose OK\n";
  }

  // =====================================================
  // STOP
  // =====================================================

  else if (type == "stop") {

    msg.type =
      CommandType::Stop;

    msg.payload.emplace<
      motion_interfaces::msg::StopCommand>();

    std::cout
      << "[JSON DECODER] Stop OK\n";
  }

  // =====================================================
  // UNKNOWN
  // =====================================================

  else {

    std::cout
      << "[JSON DECODER] UNKNOWN TYPE\n";

    return std::nullopt;
  }

  return msg;
}

} // namespace udp_ros_bridge