#include "robot_sim/robot_drive.hpp"
#include <tf2/LinearMath/Quaternion.h>

RobotDrive::RobotDrive() : Node("robot_drive")
{
    broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

    t = 0.0;

    timer_ = create_wall_timer(
        std::chrono::milliseconds(50),
        std::bind(&RobotDrive::update_pose, this)
    );
}

void RobotDrive::setPose(double x, double y, double yaw)
{
    tf2::Quaternion q;
    q.setRPY(0,0,yaw);

    geometry_msgs::msg::TransformStamped tf;

    tf.header.stamp = get_clock()->now();
    tf.header.frame_id = "world";
    tf.child_frame_id = "base_link";

    tf.transform.translation.x = x;
    tf.transform.translation.y = y;
    tf.transform.translation.z = 0.0;

    tf.transform.rotation.x = q.x();
    tf.transform.rotation.y = q.y();
    tf.transform.rotation.z = q.z();
    tf.transform.rotation.w = q.w();

    broadcaster_->sendTransform(tf);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RobotDrive>());
    rclcpp::shutdown();
}
