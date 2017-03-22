#!/usr/bin/env python

import rospy
from motor_encoder_input.msg import MotorEncoderState
from geometry_msgs import Pose
from geometry_msgs import Twist

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "Sensor %s :{ x:  %s, y: %s, g: %s }", str(data.id), str(data.x), str(data.y), str(data.grip))

def listener():
    rospy.init_node('handle_subscriber', anonymous=True)
    rospy.Subscriber('handle', StressState, callback)
    rospy.loginfo("Listener is now on.")
    #input pose/twist estimation calculations here
    rospy.spin()

if __name__ == "__main__":
    listener()
