#!/usr/bin/env python

import rospy
from motor_encoder_input.msg import MotorEncoderState
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Twist

def callback(data):
    rospy.loginfo(rospy.get_caller_id())
    rospy.loginfo(data)

def listener():
    rospy.init_node('handle_subscriber', anonymous=True)
    rospy.Subscriber('handle', MotorEncoderState, callback)
    rospy.loginfo("Listener is now on.")
    #input pose/twist estimation calculations here
    rospy.spin()

if __name__ == "__main__":
    listener()
