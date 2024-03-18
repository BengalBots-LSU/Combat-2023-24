/*
Authors: Maureen Sanchez, BengalBots team, Tristan Lin
Purpose: Create an Arduino combat bot to compete in LSU Bengal Brawl against Senior Design teams
Objective: Acts as the receiver and acts out the commands that the transmitter has sent to it 

Code References:
- Moving components https://github.com/maureensanchez99/Arroyo-Seco-Duck-Race-2022
- Parsing through data https://github.com/maureensanchez99/CaSGC-Internship-2022
*/

//Libraries needed to use certain modules and functions to make this program work
#include <SPI.h>       //used to start communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Variables
RF24 radio(7, 8); // CE, CSN
//LED pins: green - 5, yellow - 6, 
//if it is connected to transmitter, one is hardwired, one is TBD

//Constant variables 
const byte address[6] = "00001";
//const int kSwitch = 9; //kill switch 
const int leftMotorPin = 24;
const int rightMotorPin = 25;
const int weaponMotorPin = 26;
const int greenLED = 5;
const int stickMidpoint_L = 511;
const int stickMidpoint_R = 502;
const int buttonMax = 1023;
const int speedMidPos = 1750;
const int speedMidNeg = 1250;

//Changing Variables
//int leftJoystick[2] = {0, 0};

Servo leftMotor, //saber motor controller
rightMotor, //saber motor controller
weaponMotor; //talon motor controller
int moveL = 0, moveR = 0;
bool toggleDir = false;
//, rightJoystick = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL = 0;  //reads the command given from the left joystick
  int jDirectionR = 0;  //reads the command given from the right joystick
  bool killButton = false;   //reads the command given from the killswitch
  bool weaponButton = false; //reads the command given from the weapon button
  bool pointTurn = false; //reads the state of the toggle button between point and sweep turns
};

Data_Package data; //Create a variable with the above structure

void moveBotPoint(int &x, int &y)
{
  int leftMotion = (x > 1500 ? x+y : x-y);
  int rightMotion = (x > 1500 ? x-y+500 : x+y-500);
  leftMotor.write(leftMotion);
  rightMotor.write(rightMotion);
}

void moveBotSweep(int &x, int &y) 
{
  //turning scheme: robot turns based on an offset given by
  //the x axis. This offset is additive when the robot is below
  //the halfway speed of and subtractive when above
    int leftMotion = 0, rightMotion = 0;

    if (x >= 1500) //robot is moving forwards
    {
      if (y < 0) //robot is turning left
      {
        leftMotion = x > speedMidPos ? x+y : x;
        rightMotion = (x > speedMidPos ? x : x-y) - 500; 
      }
      else //robot is turning right
      {
        leftMotion = x > speedMidPos ? x : x+y;
        rightMotion = (x > speedMidPos ? x-y : x) - 500;
      }
    }
    else //robot is moving backwards
    {
      if (y < 0) //robot is turning left
      {
        leftMotion = x < speedMidNeg ? x-y : x;
        rightMotion = (x < speedMidNeg ? x : x+y) + 500; 
      }
      else //robot is turning right
      {
        leftMotion = x < speedMidNeg ? x : x-y;
        rightMotion = (x < speedMidNeg ? x+y : x) + 500;
      }
    }

    leftMotor.write(leftMotion);
    rightMotor.write(rightMotion);
}


void setup() {
  Serial.begin(115200);  // boots serial monitor at ? bps
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

   //sabertooth setup
  rightMotor.attach(rightMotorPin, 1000, 2000);
  leftMotor.attach(leftMotorPin, 1000, 2000);
  weaponMotor.attach(weaponMotorPin, 1000, 2000)
  //leftJoystick[0] = map(data.jDirectionR, 0, buttonMax, 0, 180);
  //leftJoystick[1] = map(data.jDirectionL, 0, buttonMax, 180, 0);
  /*pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  pinMode(weaponMotor, OUTPUT);
  pinMode(kSwitch, OUTPUT);
  pinMode(greenLED, OUTPUT);*/
}

void loop() {
  pinMode(greenLED, LOW);
  while(radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    pinMode(greenLED, HIGH);

    if (!data.killButton) {weaponMotor.write(data.weaponButton ? 2000 : 1500);}

    if (data.pointTurn){toggleDir = !toggleDir}}
    //else if (data.weaponButton == true) 
    //{
    //    COMPLETE THIS IF AVAILABLE
    //}
    moveL = map(data.jDirectionL, 0, 1023, 1000, 2000); //these could be constant volatiles
    moveR = map(data.jDirectionR, 0, 1023, -250, 250); //these could be constant volatiles
    toggleDir ? moveBotPoint(moveL, moveR) : moveBotSweep(moveL, moveR);
  

  // testing to make sure the values are being collected accurately 
  Serial.print("a: ");
  Serial.print(data.jDirectionL);
  Serial.print(" b: ");
  Serial.print(data.jDirectionR);
  Serial.print(" c: ");
  Serial.print(data.killButton);
  Serial.print(" d: ");
  Serial.println(data.weaponButton);
  Serial.println(" "); 
  }
}
