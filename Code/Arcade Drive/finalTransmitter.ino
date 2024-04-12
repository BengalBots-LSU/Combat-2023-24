/*
Authors: Maureen Sanchez, BengalBots team
Purpose: Create an Arduino combat to compete in the LSU Bengal Brawl against senior design team
Objective: Acts as the controller and sends to the receiver arduino the direction/angle in which the bot needs to move
            acts as the button which controls the weapon to move up and down  

Code References:
- Button Code https://docs.arduino.cc/built-in-examples/digital/Button
- Moving components https://github.com/maureensanchez99/Arroyo-Seco-Duck-Race-2022
- Wireless Communication https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/ 
*/

//Libraries needed to use certain modules and functions to make this program work
#include <SPI.h>       //used for communication between Arduinos
#include <nRF24L01.h>
#include <RF24.h>

//Variables
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

//constants
const int joystickL = A1, //where the left joystick is connected
          joystickR = A2, //where the joystickB is connected
                          //choose one of the pins from 1 - 13 as long as it is not being used
          weaponPin = 2,  //number of the pushbutton pin, weaponPin = Left Joystick Switch.
          //killPin = 3;    //number of the pushbutton pin, killPin = Right Joystick Switch.
          red = 5, //controller is powered
          yellow = 6, //kill switch activated 
          green = 9; //active communication with receiver

//changing variables
int jDirectionL, jDirectionR;                  //records the direction of the joysticks that want the motors to
                                              //move a certain way
String msg, str_jDirectionL, str_jDirectionR;  //contains all the data that needs to be sent to the robot
String activateWeapon = "";// activateKill = "";                   //holds the command to see if the weapon is to be used
bool killButton = false, weaponButton = false;           //reads the pushbutton status


struct Data_Package{
  int leftJoystick;  //reads the command given from the left joystick
  int rightJoystick;  //reads the command given from the right joystick
  bool activateKill;
  bool activateWeapon;
};
//TL: This struct was inside the loop function
//The compiler knows to take this out but it doesn't
//know to not overwrite the struct every time loop() is called.
//The compiler will unroll the loop and add all of these extra load
//and store instructions.
//It also doesn't seem necessary to use all these extra variables,
//so the code can be as follows:

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
 
  pinMode(weaponPin, INPUT);  //initializes the weapon pin as an input
  //pinMode(killPin, INPUT);
  digitalWrite(weaponPin, HIGH);
  //digitalWrite(killPin, HIGH);
pinMode(red, OUTPUT);
pinMode(yellow, OUTPUT);
pinMode(green, OUTPUT);
}

void loop() {
  Data_Package data; // Create a variable with the Data_Package structure
  data.leftJoystick = analogRead(joystickL);  //reads the command given from the left joystick
  data.rightJoystick = analogRead(joystickR);  //reads the command given from the right joystick
analogWrite(red, 64);
if(data.activateKill){
    analogWrite(yellow, 64);
    analogWrite(green, 0);
}else{
    analogWrite(green, 64);
     analogWrite(yellow,0);
}
  data.activateWeapon = digitalRead(weaponPin);
  //int moveL = map(data.leftJoystick, 0, 1023, 1000, 2000); //these could be constant volatiles
  //int moveR = map(data.rightJoystick, 0, 1023, -250, 250); //these could be constant volatiles

  radio.write(&data, sizeof(Data_Package));
  Serial.print("Left joystick: ");
  Serial.print(data.leftJoystick);
  Serial.print("Right joystick: ");
  Serial.print(data.rightJoystick);
  //Serial.print(" c: ");
  //Serial.print(data.activateKill);
  Serial.print(" d: ");
  Serial.println(data.activateWeapon);
}
