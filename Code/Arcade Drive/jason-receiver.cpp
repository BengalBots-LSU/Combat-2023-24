 /*
Authors: Caleb Taylor, Maureen Sanchez, BengalBots team
Purpose: Create an Arduino combat bot to compete in LSU Bengal Brawl against Senior Design teams
Objective: Acts as the receiver and acts out the commands that the transmitter has sent to it


Code References:
- Moving components https://github.com/maureensanchez99/Arroyo-Seco-Duck-Race-2022
- Parsing through data https://github.com/maureensanchez99/CaSGC-Internship-2022
*/


//Libraries needed to use certain modules and functions to make this program work
#include < SPI.h >       //used to start communication between Arduinos
#include < nRF24L01.h >
#include < RF24.h >


    //Variables
    RF24 radio(7, 8); // CE, CSN


//changing variables
//int jDirectionL, jDirectionR;           //int variables to store individual values


//Constant variables
const byte address[6] = "00001";
int leftMotorPin1 = 2, leftMotorPin2 = 3, rightMotorPin1 = 4, rightMotorPin2 = 5, weaponMotor = 6;
int leftMotorSpeedPin = 7, rightMotorSpeedPin = 8;
int midPoint = 127; //50% duty to not run at all 

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int jDirectionL = 0;  //reads the command given from the left joystick
  int jDirectionR = 0;  //reads the command given from the right joystick
  int killButton = 0;   //reads the command given from the killswitch
  int weaponButton = 0; //reads the command given from the weapon button
};


Data_Package data; //Create a variable with the above structure


void setup() {


    Serial.begin(115200);  // boots serial monitor at ? bps
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);
    pinMode(leftMotorSpeedPin, OUTPUT);
    pinMode(rightMotorSpeedPin, OUTPUT);
    pinMode(weaponMotor, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    
    TCCR4A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
    TCCR4B = _BV(CS01); //set to 3.92KHz, can be set to 31.3KHz(CS00)
    OCR4A = 128; // 128/255 pin 6 duty
    OCR4B = 30; // 30/255 pin 7 duty

    pinMode(46, OUTPUT);
    pinMode(45, OUTPUT);
    TCCR5A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM20);
    TCCR5B = _BV(CS02); //set to 122Hz. can be set to 3.92KHz(CS10) and 31.3KHz(CS00)
    OCR5A = 30; // 30/255 pin 46 duty
    OCR5B = 128; // 128/255 pin 45 duty
}


void loop() {
    while (radio.available()) {
        radio.read(& data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure



        if (data.weaponButton == 0 && data.killButton == 1) { //checks if the weapon needs to be activated
            digitalWrite(weaponMotor, HIGH); //activate weapon
            //don't activate kill switch
        } else if (data.weaponButton == 1 && data.killButton == 0) {
            digitalWrite(data.killButton, HIGH);//activate kill switch
        } else if (data.weaponButton == 0 && data.killButton == 0) {
            //kill bot and/or turn on bot
        }
  int forwardBack = data.jDirectionL; //stores direction from left joystick
  int leftRight = data.jDirectionR; //stores direction from right joystick
//calculate motor speed based on input

  int leftMotorSpeed = forwardBack + leftRight;
  int rightMotorSpeed = forwardBack - leftRight;
//ensure motor speeds are within range of (-255,255)
  leftMotorSpeed = constrain(leftMotorSpeed, -255, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);

  drive(leftMotorSpeed, rightMotorSpeed);





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
}

void drive(int leftDirection, int rightDirection){
// maps motor direction values to PWM range(0-255) 
  int leftPWM = map(leftDirection, -255, 255, 0, 255);
  int rightPWM = map(rightDirection, -255, 255, 0, 255);

    //Controls motor speed using PWM
    analogWrite(leftMotorSpeedPin, abs(leftPWM));
    analogWrite(rightMotorSpeedPin, abs(rightPWM));
