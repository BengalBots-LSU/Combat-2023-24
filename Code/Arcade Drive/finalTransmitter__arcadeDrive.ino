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
//#include <RH_NRF24.h>  //includes the NRF24 section of the RadioHead library
#include <nRF24L01.h>
#include <RF24.h>

//Variables
//RH_NRF24 radio(7, 8);  //the radio module is plugged in to those digital pins
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001"; //address that the NRFs will be connecting at

//constants
int joystickL = A1;       //where the left joystick is connected
int joystickR = A2;       //where the joystickB is connected
const int weaponPin = 2;  //choose one of the pins from 1 - 13 as long as it is not being used
                          //number of the pushbutton pin, weaponPin = Left Joystick Switch.
const int killPin = 3;    //number of the pushbutton pin, killPin = Right Joystick Switch.
const int greenLED = 5;

//changing values
int jDirectionL, jDirectionR;                  //records the direction of the joysticks that want the motors to
                                               //move a certain way
String msg, str_jDirectionL, str_jDirectionR;  //contains all the data that needs to be sent to the robot
String activateWeapon = "", activateKill = "";                   //holds the command to see if the weapon is to be used
int killButton = 0, weaponButton = 0;           //reads the pushbutton status

void setup() {
  Serial.begin(115200); //baud rate so we can test that we are getting the values that we want
  radio.begin(); //starts the NRF modules
  radio.openWritingPipe(address); //tells the NRF modules which address to connect to
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
 
  pinMode(weaponPin, INPUT); //initializes the weapon pin as an input
  pinMode(killPin, INPUT); //initializes the kill switch pin as an input
  digitalWrite(weaponPin, HIGH); //sets the weapon pin value to "on"
  digitalWrite(killPin, HIGH); //sets the kill switch pin value to "on"
  pinMode(greenLED, OUTPUT);
  digitalWrite(greenLED, LOW);
}

void loop() {
  while(radio.available()) {
    digitalWrite(greenLED, HIGH);
  } 
  jDirectionL = analogRead(joystickL);  //reads the command given from the left joystick
  jDirectionR = analogRead(joystickR);  //reads the command given from the right joystick
  killButton = digitalRead(killPin);
  weaponButton = digitalRead(weaponPin);

  struct Data_Package{ //sets the package 
    int leftJoystick = jDirectionL;  //reads the command given from the left joystick
    int rightJoystick = jDirectionR;  //reads the command given from the right joystick
    int activateKill = killButton;
    int activateWeapon = weaponButton;
  };

  Data_Package data; // Create a variable with the above structure
  radio.write(&data, sizeof(Data_Package));
  Serial.print("a: ");
  Serial.print(data.leftJoystick);
  Serial.print(" b: ");
  Serial.print(data.rightJoystick);
  Serial.print(" c: ");
  Serial.print(data.activateKill);
  Serial.print(" d: ");
  Serial.println(data.activateWeapon);
  Serial.println(" ");
}
