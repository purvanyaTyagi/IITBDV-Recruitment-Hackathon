from launch import LaunchDescription
from launch_ros.actions import Node
import os
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command


from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

    pkg = get_package_share_directory('robot_sim')
    urdf = os.path.join(pkg, 'urdf', 'robot.urdf')
    rviz = os.path.join(pkg, 'rviz', 'config.rviz')

    robot_description = ParameterValue(
    Command(['cat ', urdf]),
    value_type=str
    )

    return LaunchDescription([

        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            parameters=[{'robot_description': robot_description}]
        ),

        Node(
            package="robot_sim",
            executable="driver",
            output="screen"
        ),

        Node(
            package="rviz2",
            executable="rviz2",
            arguments=['-d', rviz],
            output="screen"
        )
    ])
