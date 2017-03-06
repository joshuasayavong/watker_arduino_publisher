#!/usr/bin/env python

import rospy
from handle_input.msg import StressState

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + ": I heard  %s", data)

def listener():
    rospy.init_node('handle_subscriber', anonymous=True)
    rospy.Subscriber('handle', StressState, callback)
    rospy.loginfo("Listener is now on.")

    rospy.spin()

if __name__ == "__main__":
    listener()
