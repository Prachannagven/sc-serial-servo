# The Servo Motors from Hell
(This is not slander)
The SC and ST series serial servos, are an absolute piece of madness. They are extremely high quality and high torque serial servos, but my god is it pain to control them. They come default packed to operate at a whopping 1,000,000 baud rate. While most microcontrollers _can_ handle that, you enter the region where timing is now a problem.

You cannot just take an Uno and debug it to make it work, no no no, you have to change the baud rate to a lower value (using the memor table) and then finally be able to control it with a micro controller.

This repo has two sections, the python one and the arduino one. The python code is essential. Complete details may be found at https://www.waveshare.com/wiki/Bus_Servo_Adapter_(A), where at the bottom of the page the exact use of the python library can be found, but I have a few files that are a little bit more readable and helpful. The Arduino control code is (mostly) baremetal, with the exception of the SoftwareSerial library, because the Uno only has one UART port. 

# Setting Up
After installing the python library, run the "changing baud rate" code on the board. It will subseqently become much more feasible to deal with.
