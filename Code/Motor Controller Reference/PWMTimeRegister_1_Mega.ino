/*
  This utilizes timer register and changes the prescaling on them to alter the output frequencies.
  Register 0 (TCCR0A andTTCR0B) should not be used. This will change the timing in all other timing function in arduino
  pin 11 is used for SPI so we may not be able to alter regester 2

  However on Mega there are also regester 3, 4, and 5. We can use these to aviod conflicts with other libraries and spi pin 11

  Therefore on mega I recomend using registers 4 and 5 for the motor controller

  The frequencies and pins listed below still need to be tested, They are just what I expect
*/

void setup() {

pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
TCCR4A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
TCCR4B = _BV(CS01); //set to 3.92KHz, can be set to 31.3KHz(CS00)
OCR4A = 128; // 128/255 pin 6 duty
OCR4B = 30; // 30/255 pin 7 duty

pinMode(46, OUTPUT);
pinMode(45, OUTPUT);
TCCR5A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM20);
TCCR5B = _BV(CS02); //set to 122Hz. can be set to 3.92KHz(CS10) and 31.3KHz(CS00)
OCR5A = 30; // 30/255 pin 46 duty
OCR5B = 128; // 128/255 pin 45 duty

}



void loop() {
  // put your main code here, to run repeatedly:


}