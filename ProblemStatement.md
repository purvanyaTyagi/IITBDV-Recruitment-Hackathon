#  Simulation Development Hackathon — Problem Statement

> **Build your own physics simulation for a robot from scratch.**
> No physics engines. No shortcuts. Just **you, math, and code**.

---

## Table of Contents

1. [Overview](#overview)
2. [Prerequisite Reading & Documentation](#prerequisite-reading--documentation)
3. [What You Need To Do (Two Options)](#what-you-need-to-do)
4. [Understanding the Codebase](#understanding-the-codebase)
5. [How to Run Everything](#how-to-run-everything)

6. [Feature Ideas & Sections](#feature-ideas--sections)
   - [🔬 Physics](#-physics)
   - [🎮 Controls](#-controls)
   - [📡 Sensors](#-sensors)
   - [🌍 Environment](#-environment)
7. [Judging Criteria](#judging-criteria)

---

## Prerequisite Reading & Documentation

Before you start coding, read through these to understand the tools you're working with:

### Must-Read

| Topic | Link | Why |
|---|---|---|
| **URDF Tutorial** | [docs.ros.org/en/humble/Tutorials/Intermediate/URDF](https://docs.ros.org/en/humble/Tutorials/Intermediate/URDF/URDF-Main.html) | Learn how to define your own robot model |
| **TF2 Overview** | [docs.ros.org/en/humble/Tutorials/Intermediate/Tf2](https://docs.ros.org/en/humble/Tutorials/Intermediate/Tf2/Tf2-Main.html) | Understand how coordinate frames and transforms work |
| **RViz User Guide** | [docs.ros.org/en/humble/Tutorials/Intermediate/RViz](https://docs.ros.org/en/humble/Tutorials/Intermediate/RViz/RViz-Main.html) | Know your visualisation tool |
| **ROS 2 Nodes & Topics** | [docs.ros.org/en/humble/Tutorials/Beginner-CLI-Tools](https://docs.ros.org/en/humble/Tutorials/Beginner-CLI-Tools.html) | Core ROS 2 pub/sub concepts |

### Recommended

| Topic | Link | Why |
|---|---|---|
| **Euler Integration** | [en.wikipedia.org/wiki/Euler_method](https://en.wikipedia.org/wiki/Euler_method) | The simplest way to step your simulation forward in time |
| **Rigid Body Dynamics** | [en.wikipedia.org/wiki/Rigid_body_dynamics](https://en.wikipedia.org/wiki/Rigid_body_dynamics) | Understand forces, torques, and motion for solid objects |
| **Differential Drive Kinematics** | [rossum.sourceforge.net/papers/DiffSteer](http://rossum.sourceforge.net/papers/DiffSteer/) | The math behind how two-wheeled robots move |
| **C++ Reference** | [cppreference.com](https://en.cppreference.com/) | Standard library reference for C++ |

## Overview

You are given a minimal robot simulation environment built on **ROS 2 Humble** and **RViz**. The current codebase spawns a simple differential-drive robot (defined in a URDF file) and moves it along a pre-programmed infinity (∞) shaped trajectory — **no physics involved**.

**Your goal**: Replace that scripted trajectory with a **real-time physics simulation** that you write **entirely from scratch**. You will compute forces, acceleration, velocity, and position yourself and feed the results back into the visualiser to see your robot move.

You are also encouraged to design your own robot by writing your own **URDF file** — give it wheels, sensors, arms, whatever you want!

---

## What You Need To Do

You have **two options**. Pick the one that excites you most.

---

### Option A — ROS 2 + RViz (Provided Codebase)

Use the **existing C++ ROS 2 project** provided in this repository. You will:

1. **Write your own URDF file** — Design your own robot model (body, wheels, sensors, etc.) and place it in `workspace/src/robot_sim/urdf/`.
2. **Implement physics from scratch** — Open `workspace/src/robot_sim/src/trajectory.cpp` and replace the dummy infinity-shaped trajectory with your own physics simulation logic.
3. **Visualise in RViz** — The launch file already opens RViz for you. Your robot will appear and move based on the pose you compute.

You call `setPose(x, y, yaw)` every 50 ms to tell the visualiser where the robot is. **You** decide where that position comes from — by computing forces, integrating acceleration, applying friction, reading controls, etc.

---

### Option B — Bring Your Own Engine

Don't want to use ROS? **Build the entire thing in your own 3D rendering environment.** You can use:

| Engine / Framework | Language | Notes |
|---|---|---|
| **Three.js** | JavaScript | Browser-based 3D — great for web demos |
| **Godot Engine** | GDScript / C# | Open-source game engine with physics support |
| **Unity** | C# | Industry-standard, rich ecosystem |
| **Unreal Engine** | C++ / Blueprints | High-fidelity rendering |
| **Pygame / Pyglet** | Python | Simple 2D/3D — good for quick prototypes |
| **Bevy** | Rust | Modern ECS-based engine |
| **raylib** | C/C++ | Lightweight and easy to pick up |

**Rules for Option B:**
- You must still **implement physics from scratch** — don't just enable Unity's Rigidbody or Godot's physics engine and call it a day.
- You must **design your own robot/vehicle model** (mesh, sprite, or any visual representation).
- You must **render the simulation in real time** so we can see it running.
- You should demonstrate at least the same feature depth as Option A (physics, controls, sensors, environment — see below).

---

## Understanding the Codebase

> This section is for **Option A** participants using the provided ROS 2 project. Option B participants can skip this, but reading it may still give you architecture ideas.

### Directory Structure

```
docker-Hackathon/
├── Dockerfile                              # Builds the ROS 2 + Gazebo Docker image
├── docker-compose.yml                      # Starts the container
├── DockerSetup.md                          # Setup guide (Docker, WSL, etc.)
├── ProblemStatement.md                     # ← You are here
├── images/                                 # Screenshots used in DockerSetup.md
│
└── workspace/
    └── src/
        └── robot_sim/                      # ← THE ROS 2 PACKAGE YOU WILL MODIFY
            ├── CMakeLists.txt              # Build configuration
            ├── package.xml                 # Package metadata & dependencies
            │
            ├── include/
            │   └── robot_sim/
            │       └── robot_drive.hpp     # Header file — class declaration
            │
            ├── src/
            │   ├── main.cpp                # Node setup, timer, setPose()
            │   └── trajectory.cpp          # ★ YOUR MAIN WORK GOES HERE ★
            │
            ├── urdf/
            │   └── robot.urdf             # Robot model definition (replace with your own!)
            │
            ├── rviz/
            │   └── config.rviz            # RViz display configuration
            │
            └── launch/
                └── sim.launch.py          # Launch file — starts everything
```

---

### The C++ Class — `RobotDrive`

#### Header: `include/robot_sim/robot_drive.hpp`

```cpp
class RobotDrive : public rclcpp::Node
{
public:
    RobotDrive();

    void setPose(double x, double y, double yaw);   // Sends position to RViz
    void update_pose();                              // Called every 50 ms — YOUR CODE HERE

private:
    double t;                                        // Time variable

    rclcpp::TimerBase::SharedPtr timer_;              // 50 ms wall timer
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;  // TF broadcaster
};
```

**Key points:**
- `RobotDrive` inherits from `rclcpp::Node` — it's a ROS 2 node.
- A **timer** fires every **50 milliseconds**, calling `update_pose()`.
- You have a time variable `t` that you can use however you want.
- You can **add your own member variables** (velocity, acceleration, mass, forces, etc.) to this class.

#### Implementation: `src/main.cpp`

This file contains:
- The **constructor** — sets up the TF broadcaster and 50 ms timer.
- `setPose(x, y, yaw)` — converts your `(x, y, yaw)` into a TF transform and broadcasts it so RViz can render the robot at that position.
- `main()` — standard ROS 2 spin.

**You generally don't need to modify `main.cpp`**, but you **can** if you need to (e.g., adding subscribers for keyboard input, publishers for sensor data).

#### Your Workspace: `src/trajectory.cpp`

This is where you write your physics:

```cpp
#include "robot_sim/robot_drive.hpp"
#include <cmath>

// THIS FUNCTION RUNS EVERY 50ms AFTER THE SIMULATION IS STARTED
void RobotDrive::update_pose()
{
    t += 0.05;

    double a = 2.0;

    // infinity trajectory (current placeholder — REPLACE THIS)
    double x = a * sin(t);
    double y = a * sin(t) * cos(t);

    double dx = a * cos(t);
    double dy = a * cos(2*t);
    double yaw = atan2(dy, dx);

    setPose(x, y, yaw);  // USE THIS FUNCTION TO UPDATE THE POSE OF YOUR ROBOT
}
```

**What to change:** Delete the infinity math. Replace it with your own physics simulation. Compute position from forces, acceleration, and velocity. Then pass the result to `setPose(x, y, yaw)`.

---

### The URDF File — `urdf/robot.urdf`

The current URDF defines a simple box-shaped body with two cylinder wheels and a caster:

```xml
<robot name="infinity_robot">
    <link name="base_link">       <!-- Box body: 0.4 x 0.3 x 0.1 m -->
    <link name="left_wheel">      <!-- Cylinder wheel -->
    <link name="right_wheel">     <!-- Cylinder wheel -->
    <link name="front_caster">    <!-- Sphere caster -->
    <!-- Joints connecting them together -->
</robot>
```

**You should write your own URDF!** Add more links, change shapes and sizes, add colours/materials, mount sensors, etc. The `base_link` frame is what `setPose()` moves — everything else is relative to it.

---

### The Build System — `CMakeLists.txt`

If you add **new `.cpp` source files**, you must register them in `CMakeLists.txt`:

```cmake
# Currently:
add_executable(driver src/main.cpp src/trajectory.cpp)

# If you add a new file, e.g., src/physics.cpp:
add_executable(driver src/main.cpp src/trajectory.cpp src/physics.cpp)
```

If you add **new ROS 2 dependencies** (e.g., `sensor_msgs`, `std_msgs`), add them to both `CMakeLists.txt` and `package.xml`.

---

### The Launch File — `launch/sim.launch.py`

Starts three ROS 2 nodes:
1. **`robot_state_publisher`** — reads your URDF and publishes the robot's joint/link tree.
2. **`driver`** — your compiled C++ executable (the `RobotDrive` node).
3. **`rviz2`** — the visualisation tool.

You can modify this file to launch additional nodes (e.g., a keyboard teleop node).

---

## How to Run Everything

### Initial Setup

> **IMPORTANT — Read the setup guide first!**
> Before doing anything below, open [DockerSetup.md](DockerSetup.md) and follow it **step by step**. It walks you through installing Docker, setting up WSL (on Windows), cloning the repo, and verifying everything works. **Do not skip it** — most build issues come from an incomplete setup.

Follow the [DockerSetup.md](DockerSetup.md) to install Docker and set up your environment (Windows WSL or native Linux).

### Build & Launch Workflow

```
┌──────────────────────────────────────────────────────────┐
│  1. EDIT your source code on your host machine           │
│     (trajectory.cpp, robot.urdf, etc.)                   │
│                                                          │
│  2. REBUILD the Docker container                         │
│     $ docker compose build                               │
│                                                          │
│  3. START the container                                  │
│     $ docker compose up                                  │
│                                                          │
│  4. ENTER the container (new terminal)                   │
│     $ docker exec -it ros2-humble-ignition bash          │
│                                                          │
│  5. LAUNCH the simulation                                │
│     $ ros2 launch robot_sim sim.launch.py                │
│                                                          │
│  6. SEE your robot move in the RViz window!              │
└──────────────────────────────────────────────────────────┘
```

**Step by step:**

1. **Build the container** (run from the repository root where `docker-compose.yml` lives):
   ```bash
   docker compose build
   ```
   This compiles your C++ code inside the container. **Rebuild every time you change source code.**

2. **Start the container:**
   ```bash
   docker compose up
   ```
   Keep this terminal running.

3. **Open another terminal** and enter the running container:
   ```bash
   docker exec -it ros2-humble-ignition bash
   ```

4. **Launch the simulation** inside the container:
   ```bash
   ros2 launch robot_sim sim.launch.py
   ```

5. **RViz will open** and you'll see your robot moving based on whatever logic you wrote in `update_pose()`.

> **Tip:** The edit → rebuild → launch cycle is your development loop. Get comfortable with it!

---

### For Option B

| Engine | Getting Started |
|---|---|
| **Three.js** | [threejs.org/docs](https://threejs.org/docs/) |
| **Godot** | [docs.godotengine.org](https://docs.godotengine.org/en/stable/getting_started/introduction/index.html) |
| **Unity** | [learn.unity.com](https://learn.unity.com/) |
| **Bevy** | [bevyengine.org/learn](https://bevyengine.org/learn/) |
| **raylib** | [raylib.com](https://www.raylib.com/) |

---

## Feature Ideas & Sections

Below are categories of features you can implement. **You don't have to do all of them** — focus on what interests you most and go deep. Quality over quantity.

---

### Physics

This is the **core** of the hackathon. Implement real physics from first principles.

#### Basics (Start Here)
- **Mass & Inertia** — Give your robot a mass. `F = ma` is your best friend.
- **Gravity** — Apply a constant downward force. If you're on a flat plane, gravity keeps the robot grounded; on a slope, it causes sliding.
- **Euler Integration** — Use `v += a * dt` and `x += v * dt` each timestep (dt = 0.05s) to step the simulation forward.

#### Intermediate
- **Friction** — Static and kinetic friction. The robot shouldn't slide forever on a surface. Implement a friction coefficient `μ` and compute friction forces opposing motion.
- **Air Drag** — `F_drag = -½ * ρ * Cd * A * v²` — at higher speeds the robot should slow down due to air resistance.
- **Normal Forces** — Compute the reaction force from the ground surface.
- **Collision Detection** — Detect when the robot hits walls or obstacles and respond (stop, bounce, slide).
- **Rotational Dynamics** — Torque, angular acceleration, moment of inertia. Make turning feel realistic.

#### Advanced
- **Spring-Damper Suspension** — Model a suspension system between the body and wheels. `F = -kx - bv`.
- **Verlet / RK4 Integration** — More accurate integrators than basic Euler (less energy drift).
- **Multi-Body Dynamics** — If your robot has articulated parts (arms, trailers), simulate the physics of each body and their constraints.

---

### Controls

Let the user drive the robot interactively!

#### Basics
- **WASD Keyboard Control** — `W` = forward force, `S` = backward, `A` = turn left, `D` = turn right.
- **Differential Drive** — Left and right wheel speeds. Different speeds = turning. Equal speeds = straight. Opposite speeds = spin in place.

#### Intermediate
- **Speed Limiting** — Cap the maximum velocity. Real motors have torque curves.
- **Acceleration Curves** — Don't instantly apply full speed — ramp up/down smoothly.
- **Braking** — A separate brake force that decelerates the robot faster than just releasing the throttle.

#### Advanced
- **PID Controller** — Implement a PID loop to track a target speed or heading. Tune Kp, Ki, Kd.
- **Ackermann Steering** — Front-wheel turning geometry for car-like robots. Inner wheel turns more than outer.
- **Path Following** — Given a set of waypoints, make the robot autonomously follow the path using a pure pursuit or Stanley controller.
- **Gamepad / Joystick Support** — Read input from a USB controller for analog control.

> **ROS 2 Tip:** You can subscribe to `/cmd_vel` (type `geometry_msgs/msg/Twist`) to receive velocity commands from keyboard teleop packages like `teleop_twist_keyboard`.

---

### Sensors

Simulate sensors and publish their data. This is where robotics gets interesting.

#### Basics
- **IMU (Inertial Measurement Unit)** — Report angular velocity and linear acceleration. Publish as `sensor_msgs/msg/Imu`.
- **Wheel Encoders / Odometry** — Track how much each wheel has rotated. Compute odometry (estimated position from wheel rotation). Publish as `nav_msgs/msg/Odometry`.

#### Intermediate
- **LIDAR** — Cast rays from the robot in a fan pattern, measure the distance to the nearest obstacle for each ray. Publish as `sensor_msgs/msg/LaserScan`.
- **GPS (Simulated)** — Report the robot's absolute position, optionally with noise/drift. Publish as `sensor_msgs/msg/NavSatFix`.
- **Bumper / Contact Sensor** — Detect physical contact with obstacles.

#### Advanced
- **Camera (Simulated)** — Render a simple view from the robot's perspective (even a 2D top-down view counts).
- **Sensor Noise & Drift** — Real sensors aren't perfect. Add Gaussian noise to all your sensor readings. Add bias drift to the IMU. Make the GPS jump around.
- **Sensor Fusion** — Combine IMU + wheel odometry + GPS using a Kalman filter or complementary filter to get a better position estimate.

---

### Environment

Build the world your robot lives in.

#### Basics
- **Flat Ground Plane** — A simple infinite flat surface at `z = 0`.
- **Static Obstacles** — Place boxes, cylinders, or walls in the world. Detect collisions with them.

#### Intermediate
- **Ramps & Slopes** — Inclined surfaces where gravity has a sliding component. The steeper the ramp, the harder to climb.
- **Different Surface Materials** — Areas with different friction coefficients — ice (low μ), asphalt (medium μ), gravel (high μ + extra drag).
- **Heightmap Terrain** — Define a 2D grid of height values. The robot follows the terrain elevation as it moves.

#### Advanced
- **Dynamic Obstacles** — Moving obstacles that the robot must avoid.
- **Wind Forces** — A global or localised wind force that pushes the robot sideways.
- **Water / Mud Zones** — Areas that apply heavy drag or slow the robot down significantly.
- **Destructible Terrain** — Terrain that deforms when the robot drives over it (e.g., tire tracks in soft ground).

---

## Judging Criteria

| Criteria | Weight | Description |
|---|---|---|
| **Physics Accuracy** | 30% | How realistic and well-implemented is your physics simulation? |
| **Feature Depth** | 25% | How many of the above features did you implement? How deep did you go? |
| **Code Quality** | 15% | Clean, well-structured, documented code. Good use of classes and separation of concerns. |
| **Robot Design** | 10% | Creativity and complexity of your URDF / 3D model. |
| **Presentation** | 10% | Can you demo and explain your simulation clearly? |
| **Bonus / Creativity** | 10% | Did you do something unexpected or particularly impressive? |

---

## Final Notes

- **Start simple.** Get basic `F = ma` working with keyboard control before adding friction, drag, and sensors.
- **Test often.** Use the build → launch → observe loop frequently. Don't write 500 lines and then try to compile.
- **Read the docs.** The ROS 2 and URDF docs are your best friends. A few minutes of reading saves hours of debugging.
- **Ask for help.** If you're stuck on Docker, ROS, or C++ build errors — ask the mentors. We want you spending time on physics, not fighting build tools.
- **Have fun.** This is a hackathon, not an exam. Break things. Experiment. Build something cool. 

---
