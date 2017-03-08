How To Setup Project:

1.Clone this repository:

"git clone https://github.com/joshuasayavong/watker\_arduino\_publisher.git"

2.Install bluez on your Ubuntu machine, used for BT interfacing: 

"sudo apt-get install bluez"

3.Install bluepy on pip, used for python to BT interfacing:

"pip install bluepy"

4.Copy catkin package called "handle\_input" from the git repository in your ros src folder (assume ~/ is ros project directory):

"cd ~/src"

"cp -rf /watker\_arduino\_publisher/handle\_input/ ."

5.Build the package:

"catkin\_make"

6.Run roscore:

"roscore"

7.Open following arduino file from the following folder:

"/watker\_arduino\_publisher/arduino/arduino.ino"

8.Change the pre-compiled property values for the arduino.ino file to the arduino specs:

"XPIN, YPIN, GPIN, ID"

9.Upload the Arduino file:

"Ctrl-U"

10.In another terminal, run the subscriber:
"python handle\_subscriber.py"

11.In another terminal, find the mac address of each bluno using the following command:

"sudo hcitool lescan"

12.Run the publishers. Note one publisher is needed for each connected bluno:
"python handle\_publisher.py {mac-address}"
