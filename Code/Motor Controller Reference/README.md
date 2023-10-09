# Summary
This is code created by Marco that we will reference when we work with the Sabertooth Motor Controller


This code can allow PWM control at different frequencies. This is an alternative to manually creating a wave with digitalWrite() and delay() functions. Because there are no delay() functions in this it will speed up the program. 

It works by directly accessing the hardware timer registers in the Arduino. There are prescalers that we change the value to achieve different frequencies. Here is a link that helped me understand the topic a little more: 

https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm

All of the uno version of the code has been tested on an oscilloscope to check for accuracy. The mega version still needs to be reviewed.
