How To Setup Project:

Clone this repository:
"git clone https://github.com/joshuasayavong/watker\_arduino\_publisher.git"

Install bluez on your Ubuntu machine, used for BT interfacing: 
"sudo apt-get install bluez"

Install pygatt on pip, used for python to BT interfacing:
"pip install pygatt"
"pip install "pygatt[GATTTOOL]""
"pip install git+https://github.com/peplin/pygatt"

Create catkin package called "handle\_input" in your ros src folder:
"cd ~/catkin\_ws/src"
"catkin\_create\_pkg handle\_input std\_msgs rospy roscpp"

Place the correct files in the folder:
"TBD"

Build the package:
"catkin\_make"

Run roscore:
"roscore"

Upload following arduino file from the following folder:
"~/arduino/arduino.ino"

In another terminal, run the publisher:
"python handle\_publisher.py {mac-address}"

In another terminal, run the subscriber:
"python handle\_subscriber.py"
