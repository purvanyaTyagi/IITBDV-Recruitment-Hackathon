#include "robot_sim/robot_drive.hpp"
#include <cmath>

// THIS FUNCTION RUNS EVERY 50ms AFTER THE SIMULATION IS STARTED
void RobotDrive::update_pose()
{
   static double x = 0.0;
static double y = 0.0;
static double vx = 0.0;
static double vy = 0.0;

double dt = 0.05;   // timestep (50 ms)

// robot parameters
double mass = 5.0;
double force = 10.0;

// acceleration from F = ma
double ax = force / mass;
double ay = 0.0;

// update velocity
vx += ax * dt;
vy += ay * dt;

// simple friction
double friction = 0.98;
vx *= friction;
vy *= friction;

// update position
x += vx * dt;
y += vy * dt;

// compute heading
double yaw = atan2(vy, vx);

// send pose to RViz
setPose(x, y, yaw);
}
