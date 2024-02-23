/*
Authors: Maureen Sanchez, BengalBots team
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

//Changing Variables
int leftJoystick = 0, rightJoystick = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL = 0;  //reads the command given from the left joystick
  int jDirectionR = 0;  //reads the command given from the right joystick
  bool killButton = false;   //reads the command given from the killswitch
  bool weaponButton = false; //reads the command given from the weapon button
};

Data_Package data; //Create a variable with the above structure

void moveBot(int left, int right){
  if(left > leftJoystick && right > 502) { //tells the bot to move forward
    digitalWrite(leftMotor, HIGH); //left motor moves forward
    digitalWrite(rightMotor, HIGH);//right motor moves forward
  } else if (left < leftJoystick && right < 502){ //tells the bot to move backward
    digitalWrite(leftMotor, LOW);  //left motor moves backward
    digitalWrite(rightMotor, LOW); //right motor moves backward
  } else if (left > leftJoystick && right == 502){ //tells the bot to turn right
    digitalWrite(leftMotor, HIGH); //left motor moves forward
    digitalWrite(rightMotor, LOW); //right motor moves backward
  } else if (left == leftJoystick && right > 502){ //tells the bot to turn left
    digitalWrite(leftMotor, LOW);  //left motor moves backward
    digitalWrite(rightMotor, HIGH);//right motor moves forward
  }else if (left == leftJoystick && (right == 502 || right == 503)) { //tells the bot to nothing/don't move
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

    if(data.weaponButton == false && data.killButton == true){ //checks if the weapon needs to be activated 
      digitalWrite(weaponMotor, HIGH); //activate weapon
      //don't activate kill switch
    } else if(data.weaponButton == true && data.killButton == false){ 
      digitalWrite(data.killButton, HIGH);//activate kill switch
    } else if(data.weaponButton == false && data.killButton == false){
      //kill bot and/or turn on bot
    }

    moveBot(data.jDirectionL, data.jDirectionR);
 }

  /* testing to make sure the values are being collected accurately 
  Serial.print("a: ");
  Serial.print(data.jDirectionL);
  Serial.print(" b: ");
  Serial.print(data.jDirectionR);
  Serial.print(" c: ");
  Serial.print(data.killButton);
  Serial.print(" d: ");
  Serial.println(data.weaponButton);
  Serial.println(" "); */
}
