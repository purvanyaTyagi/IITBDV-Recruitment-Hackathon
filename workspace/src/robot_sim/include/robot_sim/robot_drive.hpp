#ifndef ROBOT_DRIVE_HPP
#define ROBOT_DRIVE_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/transform_broadcaster.h>

class RobotDrive : public rclcpp::Node
{
public:
    RobotDrive();

    void setPose(double x, double y, double yaw);
    void update_pose();

private:
    double t;

    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
};

#endif
