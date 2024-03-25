/*
Authors: Maureen Sanchez, BengalBots team, Tristan Lin
Purpose: Create an Arduino combat bot to compete in LSU Bengal Brawl against Senior Design teams
Objective: Acts as the receiver and acts out the commands that the transmitter has sent to it 

Code References:
- Moving components https://github.com/maureensanchez99/Arroyo-Seco-Duck-Race-2022
- Parsing through data https://github.com/maureensanchez99/CaSGC-Internship-2022
*/

/*
3/18 Note:
TL: Motor output M1 is at a constant 15V and does not respond to stick input.
Motor output M2 behaves as expected. This side is good to go.
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
const int kSwitch = 9; //kill switch 
const int leftMotorPin = 4;
const int rightMotorPin = 13;
const int weaponMotorPin = 2;
const int greenLED = 5;
const int stickMidpoint_L = 511;
const int stickMidpoint_R = 502;
const int buttonMax = 1023;
const int speedMidPos = 45;
const int speedMidNeg = 135;

//Changing Variables
//int leftJoystick[2] = {0, 0};

Servo leftMotor, //saber motor controller
rightMotor, //saber motor controller
weaponMotor; //talon motor controller
int moveL = 0, moveR = 0, outL = 90, outR = 90;
bool toggleDir = false;
//, rightJoystick = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL;  //reads the command given from the left joystick
  int jDirectionR;  //reads the command given from the right joystick
  bool killButton;   //reads the command given from the killswitch
  bool weaponButton; //reads the command given from the weapon button
  bool pointTurn; //reads the state of the toggle button between point and sweep turns
};

void moveBotPoint(int &x, int &y, int &a, int &b)
{
  //Turning scheme: Both sides of the robot get the
  //same offset in opposite directions. This way, the robot turns in place.
 if (x > 95)
 {
   a = y < 0 ? x+y : x-y;
 }
  else if (x < 85)
  {
    a = y > 0 ? x+y : x-y;
  }
  else
  {
    a = 90;
  }
   b = map(a, 0, 180, 180, 0);
}

void moveBotSweep(int &x, int &y, int &a, int &b) 
{
  //Turning Scheme: robot turns based on an offset given by
  //the x axis. This offset is additive when the robot is below
  //the halfway speed of and subtractive when above.
  //This way, the robot can move at its maximum speed.

    if (x > 95) //robot is moving forwards
    {
      if (y < 0) //robot is turning left
      {
        a = x > speedMidPos ? x+y : x;
        b = x > speedMidPos ? x : x-y; 
      }
      else if (y > 0)//robot is turning right
      {
        a = x > speedMidPos ? x : x+y;
        b = x > speedMidPos ? x-y : x;
      }
      else
      {
        a = x;
        b = x;
      }
    }
    else if (x < 85)//robot is moving backwards
    {
      if (y < 0) //robot is turning left
      {
        a = x < speedMidNeg ? x-y : x;
        b = x < speedMidNeg ? x : x+y; 
      }
      else if (y > 0)//robot is turning right
      {
        a = x < speedMidNeg ? x : x-y;
        b = x < speedMidNeg ? x+y : x;
      }
      else
      {
        a = x;
        b = x;
      }
    }
    else
    {
      a = 90;
      b = 90;
    }
}


void setup() {
  Serial.begin(115200);  // boots serial monitor at ? bps
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

   //sabertooth setup
  //leftJoystick[0] = map(data.jDirectionR, 0, buttonMax, 0, 180);
  //leftJoystick[1] = map(data.jDirectionL, 0, buttonMax, 180, 0);
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
  pinMode(weaponMotorPin, OUTPUT);
  pinMode(kSwitch, OUTPUT);
  pinMode(greenLED, OUTPUT);
  rightMotor.attach(rightMotorPin, 1000, 2000);
  leftMotor.attach(leftMotorPin, 1000, 2000);
  weaponMotor.attach(weaponMotorPin, 1000, 2000);
  rightMotor.write(90);
  leftMotor.write(90);
  weaponMotor.write(90);
}

void loop() {
  Data_Package data; //Create a variable with the above structure
  pinMode(greenLED, LOW);
  while(radio.available() && leftMotor.attached() && rightMotor.attached() && weaponMotor.attached()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    pinMode(greenLED, HIGH);

    if (!data.killButton) {weaponMotor.write(data.weaponButton ? 2000 : 1500);}

    if (data.pointTurn){toggleDir = !toggleDir;}
    //else if (data.weaponButton == true) 
    //{
    //    COMPLETE THIS IF AVAILABLE
    //}
    moveL = map(data.jDirectionL, 0, 1024, 0, 180); //these could be constant volatiles
    moveR = map(data.jDirectionR, 0, 1024, -45, 45); //these could be constant volatiles
    toggleDir ? moveBotPoint(moveL, moveR, outL, outR) : moveBotSweep(moveL, moveR, outL, outR);
    leftMotor.write(outL);
    rightMotor.write(outR);

  // testing to make sure the values are being collected accurately 
  /* Serial.print("a: ");
  Serial.print(data.jDirectionL);
  Serial.print(" b: ");
  Serial.print(data.jDirectionR);
  Serial.print(" c: ");
  Serial.print(data.killButton);
  Serial.print(" d: ");
  Serial.println(data.weaponButton);
  Serial.print(" e: ");
  Serial.print(moveL);
  Serial.print(" f: ");
  Serial.print(moveR);
  Serial.println(" "); */
  }
}
