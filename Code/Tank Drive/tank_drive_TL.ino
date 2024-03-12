#include <SPI.h>       //used to start communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Variables
RF24 radio(7, 8); // CE, CSN

//Constant variables
const byte address[6] = "00001";
const int leftMotorPin = 2, rightMotorPin = 3, weaponMotor = 4, kSwitch = 9, greenLED = 5;
//int leftJoystick = 513, rightJoystickA = 502, rightJoystickB = 503;
//change
int buttonMax = 1023;
Servo LM1, RM1;
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

void loop() {

  //checks radio availability and reads the sent data
  if (radio.available()) 
  {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
  }

 
  if (data.killButton == 0)
  {
    //kill bot and/or turn on bot

  }
  
  digitalWrite(weaponMotor, (data.weaponButton == 1 ? HIGH : LOW));

  //Map the button values to make the range from 0 - 180. Assumes right and left motors will be opposite directions. Which the toLow and toHigh value if the direction needs to be changed
  int right = map(data.jDirectionR, 0, buttonMax, 0, 180);
  int left = map(data.jDirectionL, 0, buttonMax, 180, 0);

  //give power to the motors from 0 (reverse) to 180 (forward)
  RM1.write(right);
  LM1.write(left);
  //in ms
  delay(20);

}
