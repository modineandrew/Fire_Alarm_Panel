Hello there! It seems you have come to look at my code for my custom fire alarm control panel. 
NOTE: I wrote this code in 2018, when I was just getting started with Arduino, so some of this code is bad, incomplete, or non-functional.

Usage:
I do not care how you use this, as long as you do not make money off of it.

How to make this work.
1. You will need an Arduino Uno, along with a lot of breadboard wires, resistors, breadboards, and patience.
2. All inputs must be tied to ground through a 10K resistor, to avoid the Arduino doing random things.
3. Hook up everything according to this pinout:

LCD: rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2
A0: Silence button
A1: Reset button
A2: Your Zone 1 input. This only supports conventional pull stations.
8: Your Zone 2 input. This only supports conventional pull stations. This zone is also for supervisory only.
7: Zone Disable button. This is an input that will disable zones as you continue to press it, and then it will wrap around and everything will be active again.
6: Alarm indicator LED. Activates upon alarm condition.
A4: Alarm Output. This can go into any 5 volt activated device that doesn't draw more power than Arduino can provide.
A5: External annunciator input. I currrently do not remember the annunciator circuit, but whenever I do remember it, I will update this.
9 and 10: Zone power, 9 is for zone 1, 10 is for zone 2. These will provide 5 volts DC to a pull station if it is enabled. Without this, zone disabling would not work and the panel would get stuck in an activated loop.
A3: Pre-Alarm output. If pre-alarm duration is greater than 0 milliseconds, this will provide output during that period.

4. Upload this sketch to the Arduino! Everything should work, but you may have to slightly modify my code to fit your needs.

Thank you for checking out my fire alarm panel code, and happy tinkering! If there are any problems, please feel free to let me know.
Also, I plan to add more comments to this in the future, but I just didn't feel like diving into this code to figure out what everything does again.