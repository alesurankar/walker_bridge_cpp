cmake_minimum_required(VERSION 3.8)
project(walker_bridge_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# RVIZ VERSION
add_executable(walker_bridge_cpp
  src/walker_bridge.cpp
  src/udp_receiver.cpp
)

ament_target_dependencies(walker_bridge_cpp
  rclcpp
  std_msgs
  nlohmann_json
  Boost
)

target_include_directories(walker_bridge_cpp 
  PUBLIC
  include
  ${Boost_INCLUDE_DIRS}
)

# Install
install(TARGETS walker_bridge_cpp
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(DIRECTORY include/
  DESTINATION include
)

ament_package()