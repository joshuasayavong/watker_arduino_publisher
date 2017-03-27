#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16

def callback(data):
    rospy.loginfo(rospy.get_caller_id())
    rospy.loginfo(data)

def listener():
    
    rospy.init_node('wheel_subscriber', anonymous=True)
    rospy.Subscriber('lwheel', Int16, callback)
    rospy.Subscriber('rwheel', Int16, callback)
    rospy.loginfo("Listener is now on.")
    rospy.spin()

if __name__ == "__main__":
    listener()
