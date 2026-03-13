#include "robot_sim/robot_drive.hpp"
#include <cmath>

// THIS FUNCTION RUNS EVERY 50ms AFTER THE SIMULATION IS STARTED
void RobotDrive::update_pose()
{
    t += 0.05;

    double a = 2.0;

    // infinity trajectory
    double x = a * sin(t);
    double y = a * sin(t) * cos(t);

    // velocity for heading
    double dx = a * cos(t);
    double dy = a * cos(2*t);

    double yaw = atan2(dy, dx);

    setPose(x, y, yaw); // USE THIS FUNCTION TO UPDATE THE POSE OF YOUR ROBOT.
}
