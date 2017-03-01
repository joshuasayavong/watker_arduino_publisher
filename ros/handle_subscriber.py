#!/usr/bin/env python

from handle_input import StressState

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + ": I heard  %s", data.data)

def listener():
    rospy.init_node('handle_subscriber', anonymous=True)
    rospy.Subscriber('handle', String, callback)

    rospy.spin()

if __name__ == "__main__":
    listener()
