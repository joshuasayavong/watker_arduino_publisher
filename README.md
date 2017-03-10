# Watker Arduino Publisher

   This project implements a strain sensor and provides an interface between Arduino and ROS via Bluetooth 4.0 LE. Via the bluepy library, a ROS publisher can be run to listen to various MAC addresses. As a result, the publisher can write to a topic in the ROS domain for signal processing.

## How To Setup Project:

Clone this repository:
```
git clone https://github.com/joshuasayavong/watker_arduino_publisher.git
```
Install bluez on your Ubuntu machine, used for BT interfacing: 
```
sudo apt-get install bluez
```
Install bluepy on pip, used for python to BT interfacing:
```
pip install bluepy
```
Copy catkin package called "handle_input" from the git repository in your ros src folder (assume ~/ is ros project directory):
```
cd ~/src
cp -rf /watker_arduino_publisher/handle_input/ .
```
Build the package:
```
catkin_make
```
Run roscore:
```
roscore
```
Open following arduino file from the following folder:
```
/watker_arduino_publisher/arduino/arduino.ino
```
Change the pre-compiled property values for the arduino.ino file to the arduino specs:
```
SAMPLE_RATE, ID
```
Upload the Arduino file:
```
Ctrl-U
```
Configure Arduino baud rate using the following command:
```
+++ (New Line)
AT+UART=9600 (CR+NL)
```
In another terminal, run the subscriber:
```
python handle_subscriber.py
```
In another terminal, find the mac address of each bluno using the following command:
```
sudo hcitool lescan
```
Run the publishers. Note one publisher is needed for each connected bluno:
```
python handle_publisher.py {mac-address}
```
