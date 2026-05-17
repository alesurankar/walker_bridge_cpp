cmake_minimum_required(VERSION 3.8)
project(udp_ros_bridge)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

# Executable
add_executable(udp_bridge_node
  src/main.cpp
  src/udp_bridge_node.cpp
  src/udp_receiver.cpp
  src/command_router.cpp
  src/command_decoder.cpp
)

ament_target_dependencies(udp_bridge_node
  rclcpp
  sensor_msgs
  nlohmann_json
)

# Proper Boost linking
target_link_libraries(udp_bridge_node
  Boost::system
  Threads::Threads
)

target_include_directories(udp_bridge_node
  PUBLIC
  include
)

# Install
install(TARGETS udp_bridge_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()