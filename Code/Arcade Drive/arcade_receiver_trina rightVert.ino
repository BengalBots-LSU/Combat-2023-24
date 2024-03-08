#include <SPI.h>       //used to start communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Variables
RF24 radio(7, 8); // CE, CSN

//Constant variables
const byte address[6] = "00001";
int leftMotorPin = 2, rightMotorPin = 3, weaponMotor = 4;
//int leftJoystick = 513, rightJoystickA = 502, rightJoystickB = 503;
Servo LM1, RM1;
//change
int buttonMax = 1023;
//see DIP switch wizard to configure the pins to the servos

//figure out which direction max pwm goes: ccw or cw

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL = 0;  //reads the command given from the left joystick
  int jDirectionR = 0;  //reads the command given from the right joystick
  int killButton = 0;   //reads the command given from the killswitch
  int weaponButton = 0; //reads the command given from the weapon button
};
Data_Package data; //Create a variable with the above structure


void setup() {
  //radio setup
  Serial.begin(115200);  // boots serial monitor at ? bps
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //sabertooth setup
  LM1.attach(leftMotorPin, 1000, 2000);
  RM1.attach(rightMotorPin, 1000, 2000);

}

int mapJoystick(int directionData){
  map(directionData, 0, buttonMax, 0, 180);
}

void runServos(int r, int l){
  RM1.write(r);
  LM1.write(l);
}

void loop() {

  //checks radio availability and reads the sent data
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
  }


  if(data.weaponButton == 0 && data.killButton == 1){ //checks if the weapon needs to be activated
    //activate weapon
    //don't activate kill switch
  } else if(data.weaponButton == 1 && data.killButton == 0){
    //activate kill switch
  } else if(data.weaponButton == 0 && data.killButton == 0){
    //kill bot and/or turn on bot
  }

  //obtain joystick values and convert them into the ranges 0 - 180
  //left joystick is horizontal, right is vertical
  int yAxis = mapJoystick(data.jDirectionL);
  int xAxis = mapJoystick(data.jDirectionR);
  
  //turns the joystick values into left and right speeds
  int rightSpeed = 180 - (xAxis + yAxis);
  rightSpeed = constrain(rightSpeed, 0, 180);
  int leftSpeed = xAxis - yAxis;
  leftSpeed = constrain(leftSpeed, 0 , 180);

  //give power to the motors from 0 (reverse) to 180 (forward)
  runServos(rightSpeed, leftSpeed);
  //in ms
  delay(20);

}
