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

//Variables
RF24 radio(7, 8); // CE, CSN
//LED pins: green - 5, yellow - 6, 
//if it is connected to transmitter, one is hardwired, one is TBD

//Constant variables 
const byte address[6] = "00001";
const int leftMotor = 2; //saber motor controller
const int rightMotor = 3; //saber motor controller
const int weaponMotor = 4; //talon motor controller
const int kSwitch = 9; //kill switch 
const int greenLED = 5;
const int stickMidpoint_L = 511;
const int stickMidpoint_R = 502;

//Changing Variables
int leftJoystick[2] = {0, 0};
//, rightJoystick = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL = 0;  //reads the command given from the left joystick
  int jDirectionR = 0;  //reads the command given from the right joystick
  bool killButton = false;   //reads the command given from the killswitch
  bool weaponButton = false; //reads the command given from the weapon button
};

Data_Package data; //Create a variable with the above structure

void moveBot(int x, int y) 
{
  if (x > leftJoystick[0]) 
  { 
    digitalWrite(leftMotor, HIGH); 
    if (right > 510)
    {
        digitalWrite(rightMotor, (y == 511 ? LOW : HIGH));
    }
  } 
  else if (x == leftJoystick[0] && y > 510) 
  { 
    digitalWrite(leftMotor, LOW);
    digitalWrite(rightMotor, (y > 512 ? LOW:HIGH)); 
  } 
  else if (x < leftJoystick[0] && y < 511) 
  { 
    digitalWrite(leftMotor, LOW);  
    digitalWrite(rightMotor, LOW); 
  }
}



void setup() {
  Serial.begin(115200);  // boots serial monitor at ? bps
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  pinMode(weaponMotor, OUTPUT);
  pinMode(kSwitch, OUTPUT);
  pinMode(greenLED, OUTPUT);
}

void loop() {
  pinMode(greenLED, LOW);
  while(radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    pinMode(greenLED, HIGH);

    if (data.killButton == false) 
    {
       digitalWrite(weaponMotor, (data.weaponButton ? LOW : HIGH)); 
    }
    //else if (data.killButton == true && data.weaponButton == true) 
    //{
    //    COMPLETE THIS IF AVAILABLE
    //}

    moveBot(data.jDirectionL, data.jDirectionR);
  

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
