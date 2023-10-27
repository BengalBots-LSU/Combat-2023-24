/*#include <SPI.h>       //used to start communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>*/
#include <Servo.h>

int leftMotorPin1 = 2, leftMotorPin2 = 3, rightMotorPin1 = 4, rightMotorPin2 = 5, weaponMotor = 6;
Servo LM1, LM2, RM1, RM2;
//change
int buttonMax = 1023;

void setup() {
  //sabertooth setup
  LM1.attach(leftMotorPin1, 1000, 2000);
  LM2.attach(leftMotorPin2, 1000, 2000);
  RM1.attach(rightMotorPin1, 1000, 2000);
  RM2.attach(rightMotorPin2, 1000, 2000);

}

void loop() {
  //testing the motor controller
  int right = 45, left = 90;
  RM1.write(right);
  RM2.write(right);
  LM1.write(left);
  LM2.write(left);
  //in ms
  delay(20);
}
