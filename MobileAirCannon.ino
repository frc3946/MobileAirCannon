#define WATCHDOG_TIMEOUT WDTO_500MS
<<<<<<< HEAD
#define XBEE_BAUD 9600
#define XBEE_TIMEOUT 500   // if we have not heard from the controller in this long, robot will stop
=======
#define XBEE_BAUD 57600
#define XBEE_TIMEOUT 150
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
#define BUTTON_UP_DELAY 500
#define BUTTON_DOWN_DELAY 1500
#define FIRING_TIME 30

typedef enum {
  BUTTON0,
  BUTTON1,
  BUTTON2,
  B_SELECT,
  B_START,
  DPAD_UP,
  DPAD_LEFT,
  DPAD_DOWN,
  DPAD_RIGHT,
  B_1,
  B_4,
  B_2,
  B_3,
  RIGHTZ_1,
  RIGHTZ_2,
  LEFTZ_1,
  LEFTZ_2
  } ButtonNames;

typedef enum {
<<<<<<< HEAD
  RIGHTX,
  RIGHTY,
  LEFTX,
  LEFTY
=======
  LEFTX,
  LEFTY,
  RIGHTX,
  RIGHTY
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
} JoystickNames;

#include "./SpeedController.h"
#include "./Spike.h"
#include "./Relay.h"
//#include "./Controller.h"
#include <avr/wdt.h> //WatchDog
<<<<<<< HEAD
#include "AltSoftSerial.h"
#include "wavTrigger.h"

int buttonState[17];
int joystick[4];
char debugOut[100];
=======

int buttonState[17];
int joystick[4];

>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
int AnalogButton[2];

void birthWatchdog(uint8_t timeout) 
  {wdt_enable(timeout);}
void killWatchdog()
  {wdt_disable();}
void feedWatchdog()
  {wdt_reset();}

class Barrel {
  private:
    Relay relay;
    uint8_t firingState;
    unsigned long timer; //used to track button and track relay
  public:
    Barrel() {
      firingState = 0;
      timer = millis();
    }
    void updateFireButton(bool button) {
      switch (firingState) {
        case 0: //reset system timer
          timer = millis();
          firingState++;
          break;

        case 1: //waiting for button left up for X seconds
          if (button == true)
            timer = millis();
          else if (millis() - timer > BUTTON_UP_DELAY) {
            timer = millis();
            firingState++;
          }
          break;

        case 2: //wait for button to be pressed
          if (button == true) {
            timer = millis();
            firingState++;
          }
          break;

        case 3: //wait for button to be held down for x seconds
          if (button == false)  //if button is released then reset the system
            firingState=0;
          else if (millis() - timer > BUTTON_DOWN_DELAY) {
            relay.set(Relay::GND); //open relay and reset timer
            timer = millis();
            firingState++;
          }
          break;

        case 4: //firing! wait for timeout and then close the relay
          if (millis() - timer > FIRING_TIME) {
           relay.set(Relay::PWR); //If so, turn valve off
           firingState =0;
          }
          break;
      }
    }
    
    void attach(uint8_t pin) {
      relay.attach(pin);
      relay.set(Relay::PWR);
    }
    void shutOff() { //Closes Valve
      relay.set(Relay::PWR);
    }
};
unsigned long xbeeTimer;

//Controller controller;

Talon leftMotor1;
Talon leftMotor2;
Talon rightMotor1;
Talon rightMotor2;

Barrel leftBarrel;
Barrel rightBarrel;

//PinMap

  //21 total DIO pins
  //These two pin are used to jump over the serial coms
<<<<<<< HEAD
  uint8_t serialJumper1 = 2;// connect pin 2 to RX3
  uint8_t serialJumper2 = 3;
  uint8_t rightMotorPin1 = 4;//front
  uint8_t rightMotorPin2 = 5;//back
=======
  uint8_t serialJumper1 = 2;
  uint8_t serialJumper2 = 3;
  uint8_t rightMotorPin1 = 4;
  uint8_t rightMotorPin2 = 5;
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
  uint8_t relayBoardPwr = 6;
  uint8_t relayBoardGrnd = 7;
  uint8_t leftBarrelPin = 8;
  uint8_t rightBarrelPin = 9;
<<<<<<< HEAD
  uint8_t leftMotorPin1 = 10;//front
  uint8_t leftMotorPin2 = 11;//back
=======
  uint8_t leftMotorPin1 = 10;
  uint8_t leftMotorPin2 = 11;
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
  uint8_t compressorRelay = 12;
  uint8_t compressorSwitch = 13;
  
void setup() {
  //these two pin are used to jumper over the serial coms
  pinMode(serialJumper1, INPUT);
  pinMode(serialJumper2, INPUT);
  pinMode(relayBoardPwr, OUTPUT);
  pinMode(relayBoardGrnd, OUTPUT);
  pinMode(compressorRelay, OUTPUT);
  pinMode(compressorSwitch, INPUT);

  digitalWrite(relayBoardGrnd, LOW);
  digitalWrite(relayBoardPwr, HIGH);
  digitalWrite(compressorRelay, LOW);
  
<<<<<<< HEAD
  Serial.begin(115200);
  while(!Serial); //Wait for Serial Port to Connect. (For Leonardo Only)
  
  Serial.println("\n\rSetup!\n\r");
=======
  Serial.begin(9600);
  while(!Serial); //Wait for Serial Port to Connect. (For Leonardo Only)
  
  Serial.println("Setup!");
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba

  leftMotor1.attach(leftMotorPin1);
  rightMotor1.attach(rightMotorPin1);
  leftMotor2.attach(leftMotorPin2);
  rightMotor2.attach(rightMotorPin2);
  Serial3.begin(XBEE_BAUD);
  leftBarrel.attach(leftBarrelPin);
  rightBarrel.attach(rightBarrelPin);

  birthWatchdog(WATCHDOG_TIMEOUT);
}

long imAlivePrint = 0;


void loop() {
  feedWatchdog();

<<<<<<< HEAD
//  if (millis() - imAlivePrint > 1000)  {
//    Serial.println("\n\rAlive!\n\r");
//    imAlivePrint = millis();
//  }

  //CompressorUpdate();

  if(ControllerUpdate() == 1) { //Successful Data Retreval from Controller
    xbeeTimer = millis();
    Serial.println(xbeeTimer);
=======
  if (millis() - imAlivePrint > 1000)  {
    Serial.println("Alive!");
    imAlivePrint = millis();
  }

  CompressorUpdate();

  if(ControllerUpdate() == 1) { //Successful Data Retreval from Controller
    xbeeTimer = millis();
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
  //* Remove one of the "/" to comment out block
    //controller.printFrameData(Serial);
    Serial.print("LY: ");
  	Serial.println(buttonState[LEFTY]);
  	Serial.print("RY: ");
  	Serial.println(buttonState[RIGHTY]);
  	if(buttonState[LEFTZ_1])
  		Serial.println("L1: True");
  	else
  		Serial.println("L1: False");
  	if(buttonState[RIGHTZ_1])
  		Serial.println("R1: True");
  	else
  		Serial.println("R1: False");
	//*/
	//* Remove one of the "/" to comment out block
    leftBarrel.updateFireButton(buttonState[LEFTZ_1]);
    rightBarrel.updateFireButton(buttonState[RIGHTZ_1]);
<<<<<<< HEAD
    char debugOut[100];
    sprintf(debugOut, "LEFTY: %i\n\r", joystick[LEFTY]);
	  Serial.println(debugOut);
    leftMotor1.set(-joystick[LEFTY]);
    rightMotor1.set(-joystick[RIGHTY]);
    leftMotor2.set(joystick[LEFTY]);
    rightMotor2.set(-joystick[RIGHTY]);
	//*/
  } else {
    //not timed out
=======
	
    leftMotor1.set(joystick[LEFTY]);
    rightMotor1.set(joystick[RIGHTY]);
    leftMotor2.set(joystick[LEFTY]);
    rightMotor2.set(joystick[RIGHTY]);
	//*/
  } else {
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
    if(millis() - xbeeTimer < XBEE_TIMEOUT) { 
      leftBarrel.updateFireButton(buttonState[LEFTZ_1]);
      rightBarrel.updateFireButton(buttonState[RIGHTZ_1]);
    } else {
<<<<<<< HEAD
      //Serial.println("Timed Out");
      sprintf(debugOut, "Timed Out MS: %i", millis());
      Serial.println(debugOut);
=======
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
      leftMotor1.set(0);
      rightMotor1.set(0);
      leftMotor2.set(0);
      rightMotor2.set(0);
      leftBarrel.shutOff();
      rightBarrel.shutOff();
    }
    //controller.printErrorMessage(Serial); //Handle Controller Error Codes
  }
}

int ControllerUpdate(){
  static char controllerState[200] = "";
  static int index = 0;
  int readyFlag = 0;
  int timeToProcessFlag = 0;
  //Get a data string excluding the crc
  //Get the data string
  //Reads data string until newline is found
<<<<<<< HEAD

  //***************************************
  //added && index < 200
  while(index < 200){
    controllerState[index] = Serial3.read();   
    Serial.write(controllerState[index]);
Serial.println("this has been done inside while");
   //*****************************************************
    //removed this 8-29 from || index >=200
    if (controllerState[index] == '\n'){
      timeToProcessFlag = 1;
      Serial.write("\r\nTimeToProcess\r\n");
      break;
    }
    index++;  
  }
Serial.println("outside while");
  //Serial.write("Done reading");
  
  if (timeToProcessFlag == 0){
   // Serial.write("HERE IT IS");
=======
  while(Serial3.available()){
    controllerState[index] = Serial3.read();   
    Serial.write(controllerState[index]);
    if (controllerState[index] == '\n' || index >= 200){
      timeToProcessFlag = 1;
      Serial.write('\r');
    }
    index++;  
  }

  //Serial.write("Done reading");
  
  if (timeToProcessFlag == 0){
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
    return 0;
  }

  char * startPointer = controllerState;
  index = 0;
  //Find location where string starts, index will equal where string starts
  while (index < 200){
    if (controllerState[index] == '$'){
      break;
    }
    index++;
  }
<<<<<<< HEAD

  //Make sure that we found the beginning of a string, if not, exit
  if (controllerState[index] == '$'){
    readyFlag = 1;
//    Serial.println("NewData\r\n");
  }

  if (readyFlag == 0){
    Serial.println("BAD DATA\r\n");
    index = 0;
    return 0;
  }
 // sprintf(debugOut, "\r\nIndex: %i", index);
  Serial.println(debugOut);
  index = 0;
=======
  //Make sure that we found the beginning of a string, if not, exit
  if (controllerState[index] == '$'){
    readyFlag = 1;
    Serial.println("NewData");
  }
  if (readyFlag == 0){
    index = 0;
    return 0;
  }
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
  
//Find the crcString and link it to the crcString variable
  char crcString[10] = "";
  int crcIndex = index;
  while (crcIndex < 200){
    if (controllerState[crcIndex] == '#'){
      strncpy(crcString, &controllerState[crcIndex+1], sizeof(crcString));
      controllerState[crcIndex] = NULL;
      break;
    }
    crcIndex++;
  }
  
  short crcCalculated = crc16((unsigned char*)&controllerState[index], strlen(&controllerState[index]));
  unsigned int crcRead = atoi(crcString);
  if (crcCalculated != crcRead){
    Serial.println("BadCheckSum");
    return 0;
  }
<<<<<<< HEAD
 // Serial.println("Good CheckSum");
  //Serial.print(controllerState);
=======
   Serial.println("Good CheckSum");
  Serial.print(controllerState);
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
  parseString(controllerState);
  return 1;
}

unsigned short crc16(unsigned char* data_p, unsigned char length){
  unsigned char x;
  unsigned short crc = 0xFFFF;

  while(length--){
    x = crc >> 8 ^ *data_p++;
    x ^= x>>4;
    crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
  }
  return crc;
}

void CompressorUpdate(){
  static unsigned long lastUpdateTime = 0;
  if(millis() - lastUpdateTime >= 5000){
    lastUpdateTime = millis();
  }
  else{
    return;
  }
  if(digitalRead(compressorSwitch) == HIGH){
    digitalWrite(compressorRelay, HIGH);
  }
  else{
    digitalWrite(compressorRelay, LOW);
  }
}

void parseString(char* controllerState){
  char* tok;
  tok = strtok (controllerState+3, ",");
  
  for(int i = 3; i < 17; i++){
    buttonState[i] = atoi(tok);
    tok = strtok (NULL, ",");
    if (tok == NULL){
      return;
    }
  }
<<<<<<< HEAD
    AnalogButton[0] = atoi(tok);
  tok = strtok (NULL, ",");
  if (tok == NULL){
    return;
  }
  AnalogButton[1] = atoi(tok);
  
  for(int i = 0; i < 4; i++){
    joystick[i] = atoi(tok);
    joystick[i] = ((joystick[i] / 5.115) - 100);                      
    tok = strtok (NULL, ",");   
=======
  for(int i = 0; i < 4; i++){
    joystick[i] = atoi(tok);
    joystick[i] = ((joystick[i] / 5.115) - 100);                       // WE DID STUFF HERE
    tok = strtok (NULL, ",");
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
    if (tok == NULL){
      return;
    }
  }
  
<<<<<<< HEAD

=======
  AnalogButton[0] = atoi(tok);
  tok = strtok (NULL, ",");
  if (tok == NULL){
    return;
  }
  AnalogButton[1] = atoi(tok);
>>>>>>> 1aecab9447e1e0cd5e8447eb432cf310fa5c45ba
}

