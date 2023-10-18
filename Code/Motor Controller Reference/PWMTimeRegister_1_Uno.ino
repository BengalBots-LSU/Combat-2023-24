/*
  This utilizes a timer register and changes the prescaling on them to alter the output frequencies.
  Register 0 (TCCR0A and TTCR0B) should not be used. This will change the timing in all other timing functions in Arduino
  pin 11 is used for SPI so we may not be able to alter register 2

  Also, note that the variable names below are directly linked to the timer registers.
  analogWrite() may still be used to change the width(duty) of the PWM without reverting to the default frequency.

  I recommend using register 1 at 122Hz for talon and using pins 5 and 6 at default 980Hz for sabertooth.
  This recommendation is for Uno ONLY
*/
void setup() {

pinMode(3, OUTPUT);
pinMode(11, OUTPUT);
TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
TCCR2B = _BV(CS01); //set to 3.92KHz, can be set to 31.3KHz(CS00)
OCR2A = 128; // x/255 pin 11 duty
OCR2B = 30; // x/255 pin 3 duty

pinMode(10, OUTPUT);
pinMode(9, OUTPUT);
TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM20);
//TCCR1B = _BV(CS02); //set to 122Hz. can be set to 3.92KHz(CS10) and 31.3KHz(CS00)
TCCR1B = 4;//can also be set using intergers 0, 1, 2, and 4
OCR1A = 30; // x/255 pin 9 duty
OCR1B = 128; // x/255 pin 10 duty

//pinMode(5, OUTPUT);
//pinMode(6, OUTPUT);
//TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM20);
//TCCR0B = _BV(CS02); //set to 122Hz. can be set to 3.92KHz(CS10) and 31.3KHz(CS00)
//OCR0A = 128; // x/255 pin 6 duty
//OCR0B = 15; // x/255 pin 5 duty

}



void loop() {
  // put your main code here, to run repeatedly:


}
