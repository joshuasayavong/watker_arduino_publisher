
import time
import binascii
import sys

import rospy
from motor_encoder_input.msg import MotorEncoderState
from std_msgs.msg import Int16
from bluepy import btle
from ctypes import *

class MyDelegate(btle.DefaultDelegate):
    def __init__(self):
        btle.DefaultDelegate.__init__(self)
        # ... initialise here
    
    def parse_input(self,data):
        a = bytearray()
        for i in data:
            a.append(bytes(i))
        return a
    
    def handleNotification(self, cHandle, data):
        
        data_array = self.parse_input(data)
        
        if len(data) != 4  or data_array[0] != 0xef or data_array[1] != 0x02 :
            s = ""
            for i in data_array:
                s = s + " " + str(i)
            print("Noise: %s", s)
            return
        
        lwheel = Int16()
        data = (0xFFFF & (int(data_array[2]) << 8)) | (0XFFFF & (int(data_array[3]))) 
        
        data = c_int16(data).value

        # right encoder is configured in the negative direction
        if encoder_id == 'r':
            data = -1*data

        lwheel.data = data
        rospy.loginfo(lwheel)
        global pub
        pub.publish(lwheel)

    
def talker():
    global pub 
    global encoder_id
    pub = rospy.Publisher(encoder_id+'wheel', Int16, queue_size=10)
    rospy.init_node('publisher', anonymous=True)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        if p.waitForNotifications(1.0):
            continue

        print("Waiting...")
        rate.sleep()


if __name__ == '__main__':

    global encoder_id
    

    if len(sys.argv) != 3:
	encoder_id = 'l'
        mac = 'C8:A0:30:FA:12:9D'
    else:
	if (sys.argv[1] == 'r'):
             encoder_id = 'r'
        else:
             encoder_id = 'l'
        mac = str(sys.argv[2])

    try:
        address = mac
        p = btle.Peripheral( address )
        p.setDelegate( MyDelegate() )

    finally:
        time.sleep(1)

    try:
        talker()
    except rospy.ROSInterruptException:
        pass
