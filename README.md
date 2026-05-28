
# 📡 UDP → ROS2 Bridge Project Documentation

## Project Name: `udp_ros_bridge`

## Companion Package: `motion_interfaces`

---

# 1. System Purpose

This project implements a **real-time UDP-to-ROS2 bridge** that converts **JSON-encoded robot commands received over UDP** into strongly typed ROS2 messages.

It acts as a **gateway between external controllers (Python UDP sender, simulators, or AI planners)** and a ROS2-based humanoid control stack (MoveIt / Crocoddyl-ready pipeline).

---

# 2. High-Level Architecture

```
UDP Sender (Python)
        ↓
UDP (127.0.0.1:17945)
        ↓
UdpReceiver (Boost.Asio async I/O)
        ↓
SPSCQueue (lock-free buffer)
        ↓
UdpBridgeNode consumer thread
        ↓
JsonCommandDecoder (JSON → typed command)
        ↓
CommandRouter (dispatch by type)
        ↓
ROS2 Publishers
        ↓
ROS2 Topics (motion_interfaces messages)
```

---

# 3. Runtime Components

## 3.1 UDP Sender (External Python Script)

### File: `udp_sender.py`

Sends JSON messages via UDP to:

```
IP: 127.0.0.1
Port: 17945
```

### Behavior

#### Step 1: Initialization message (sent once)

* Type: `joint_position`
* Contains:

  * full robot joint list (30 joints)
  * initial positions

Purpose:

* Establish robot state
* Initialize downstream motion stack (e.g., MoveIt)

#### Step 2: Continuous streaming loop (every 10 seconds)

Alternates or sends:

* `cartesian_pose` commands with sinusoidal motion:

  * x oscillates: `0.3 + 0.02 * sin(t)`
  * y oscillates: `0.02 * cos(t)`
  * z fixed: `0.2`
  * orientation fixed quaternion (identity)

### Output behavior:

* Produces low-frequency trajectory updates (~0.1 Hz)
* Generates continuous Cartesian motion target stream

---

## 3.2 UDP Receiver (`UdpReceiver`)

### Technology:

* Boost.Asio UDP async socket
* Single IO thread

### Responsibilities:

1. Bind UDP socket to port `17945`
2. Asynchronously receive datagrams
3. Copy raw bytes into `UdpMessage`
4. Timestamp reception (nanoseconds)
5. Push into lock-free queue (`SPSCQueue`)

### Internal Data Structure

```cpp
struct UdpMessage {
  uint64_t receive_timestamp_ns;
  uint16_t size;
  std::byte data[1400];
};
```

### Queue Behavior:

* Capacity: 4096 messages
* Single producer (UDP thread)
* Single consumer (ROS thread)

If full:

```
[QUEUE] DROP
```

Otherwise:

```
[QUEUE] push OK, size=XXX
```

---

## 3.3 Lock-Free Queue (`SPSCQueue`)

### Type: Single Producer / Single Consumer ring buffer

### Key properties:

* Fixed capacity (power of 2)
* Lock-free atomic head/tail
* Cache-line aligned (64 bytes)
* Move-enabled push/pop

### Role in system:

Decouples:

* high-frequency UDP reception
* ROS2 processing loop

Prevents blocking in network thread.

---

## 3.4 ROS2 Node (`UdpBridgeNode`)

### Node name:

```
udp_bridge_node
```

### Responsibilities:

1. Start UDP receiver
2. Run consumer thread
3. Decode messages
4. Route commands
5. Publish ROS2 messages

---

## 3.5 Consumer Thread (`consumer_loop`)

Runs continuously:

### Loop behavior:

```
while running:
    drain all UDP messages from queue
    keep only latest message
    decode JSON → CommandMessage
    route to appropriate handler
    sleep 2ms
```

### Key design decision:

Only **latest message per cycle is processed**
→ ensures real-time control behavior (drops backlog intentionally)

---

## 3.6 JSON Decoder (`JsonCommandDecoder`)

### Input:

```cpp
(const std::byte* data, size_t size)
```

### Output:

```cpp
std::optional<CommandMessage>
```

---

## Supported JSON Schema

### Required structure:

```json
{
  "header": {
    "robot_id": 1,
    "type": "joint_position | cartesian_pose | base_velocity | stop",
    "priority": 1
  },
  "payload": { ... }
}
```

---

## Supported command types:

---

### 1. Joint Position

```json
"type": "joint_position"
```

Payload:

```json
{
  "names": [string],
  "positions": [float]
}
```

Converted into:

```cpp
motion_interfaces::msg::JointPositionCommand
```

---

### 2. Cartesian Pose

```json
"type": "cartesian_pose"
```

Payload:

```json
{
  "target_link": string,
  "frame_id": string,
  "x,y,z": float,
  "qx,qy,qz,qw": float,
  "position_gain": float,
  "orientation_gain": float,
  "is_relative": bool
}
```

Converted into:

```cpp
motion_interfaces::msg::CartesianPoseCommand
```

---

### 3. Base Velocity (supported but not used in current sender)

```json
"type": "base_velocity"
```

Payload:

```json
{
  "vx": float,
  "vy": float,
  "yaw_rate": float
}
```

---

### 4. Stop

```json
"type": "stop"
```

No payload required.

---

## Decoder Output

Returns:

```cpp
CommandMessage {
  robot_id,
  priority,
  type,
  payload (variant),
  receive_timestamp_ns
}
```

---

## 3.7 Command Router

### Purpose:

Dispatch decoded commands into strongly typed callbacks.

### Mapping:

| CommandType   | Callback                 |
| ------------- | ------------------------ |
| JointPosition | publish_joint_state()    |
| BaseVelocity  | publish_base_velocity()  |
| CartesianPose | publish_cartesian_pose() |
| Stop          | publish_stop()           |

---

## 3.8 ROS2 Publishers

Topics:

### Joint commands

```
/walker/joint_position_command
```

### Cartesian commands

```
/walker/cartesian_pose_command
```

### Base velocity

```
/walker/base_velocity_command
```

### Stop

```
/walker/stop_command
```

---

## 3.9 Message Types (`motion_interfaces`)

### Package purpose:

Defines all robot command interfaces.

---

### 1. CommandMetadata

```cpp
builtin_interfaces/Time stamp
uint32 robot_id
uint8 priority
```

---

### 2. JointPositionCommand

```cpp
CommandMetadata metadata
string[] joint_names
float64[] positions
```

---

### 3. CartesianPoseCommand

```cpp
CommandMetadata metadata
string target_link
string frame_id
bool is_relative
geometry_msgs/Pose pose
float64 position_gain
float64 orientation_gain
```

---

### 4. BaseVelocityCommand

```cpp
CommandMetadata metadata
float64 vx
float64 vy
float64 yaw_rate
```

---

### 5. StopCommand

```cpp
CommandMetadata metadata
```

---

# 4. Execution Flow (Step-by-Step)

## Startup

1. ROS2 node starts:

```
udp_bridge_node
```

2. UDP receiver binds:

```
0.0.0.0:17945
```

3. IO thread starts (Boost.Asio)

4. Consumer thread starts

---

## Runtime loop

### For each UDP packet:

1. UDP packet arrives
2. Stored in `UdpMessage`
3. Pushed into SPSCQueue
4. Consumer thread drains queue
5. Latest message selected
6. JSON decoded
7. CommandMessage created
8. Router dispatches
9. ROS2 message stamped with current time
10. Published to topic

---

# 5. Observed Runtime Output Behavior

From your logs:

### UDP reception:

```
[QUEUE] push OK, size=915
```

### JSON decoding:

```
[JSON DECODER] raw: {...}
[JSON DECODER] type = joint_position
[JSON DECODER] JointPosition OK count=30
```

### ROS publication:

```
Publishing JointPositionCommand
JOINT | ros_time=...
```

### Cartesian updates:

```
POSE | ros_time=...
```

---

# 6. Timing Characteristics

### Sender rate:

* 1 message every 5 seconds

### Bridge processing:

* ~2ms consumer loop cycle
* near real-time forwarding

### Effective behavior:

* Latest UDP message wins
* Older backlog discarded

---

# 7. Design Properties

## Real-time behavior

* Lock-free queue
* Minimal blocking in IO thread
* Latest-message-only processing

## Decoupling

* UDP thread independent from ROS thread
* JSON decoding isolated from networking

## Extensibility

* CommandRouter supports new command types easily
* Variant-based payload system

---

# 8. Failure Modes

### 1. Queue overflow

```
[QUEUE] DROP
```

→ indicates consumer too slow

---

### 2. Invalid JSON

```
[JSON DECODER] parse failed
```

---

### 3. Missing fields

```
missing header / payload
```

---

### 4. Unknown command type

```
UNKNOWN TYPE
```

---

# 9. Key System Insight (Important)

This system is NOT a strict message processor.

It is a:

> **real-time UDP motion bridge that prioritizes freshness over completeness**

Meaning:

* old messages are intentionally discarded
* only latest state is relevant
* system behaves like a control input stream, not a queue processor

---



# Start udp_bridge_node
```bash
cd ~/ros2_ws
colcon build --packages-select udp_ros_bridge
source ~/ros2_ws/install/setup.bash
ros2 run udp_ros_bridge udp_bridge_node
```


# desired architecture
```text
[ UDP Receiver ]
       ↓
[ SPSC Queue ]
       ↓
[ Decoder (pluggable) ]
       ↓
[ ROS Publisher Mapper ]
```