cmake_minimum_required(VERSION 3.8)
project(walker_bridge_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# RVIZ VERSION
add_executable(walker_bridge_cpp
  src/main.cpp
  src/walker_bridge.cpp
)

ament_target_dependencies(walker_bridge_cpp
  rclcpp
)

target_include_directories(walker_bridge_cpp 
  PUBLIC
  include
)

# Install
install(TARGETS 
  walker_bridge_cpp
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(DIRECTORY include/
  DESTINATION include
)

# Install motion data
install(DIRECTORY data/
  DESTINATION share/${PROJECT_NAME}
)

ament_package()