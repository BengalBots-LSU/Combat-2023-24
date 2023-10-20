#include <SPI.h>       //used to start communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Variables
RF24 radio(7, 8); // CE, CSN

//Constant variables
const byte address[6] = "00001";
int leftMotorPin1 = 2, leftMotorPin2 = 3, rightMotorPin1 = 4, rightMotorPin2 = 5, weaponMotor = 6;
//int leftJoystick = 513, rightJoystickA = 502, rightJoystickB = 503;
Servo LM1, LM2, RM1, RM2;
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
  LM1.attach(leftMotorPin1, 1000, 2000);
  LM2.attach(leftMotorPin2, 1000, 2000);
  RM1.attach(rightMotorPin1, 1000, 2000);
  RM2.attach(rightMotorPin2, 1000, 2000);

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

  //Map the button values to make the range from 0 - 180. Assumes right and left motors will be opposite directions. Wwitch the toLow and toHigh value if the direction needs to be changed
  int right = map(data.jDirectionR, 0, buttonMax, 0, 180);
  int left = map(data.jDirectionL, 0, buttonMax, 180, 0);

  //give power to the motors from 0 (reverse) to 180 (forward)
  RM1.write(right);
  RM2.write(right);
  LM1.write(left);
  LM2.write(left);
  //in ms
  delay(20);

}
