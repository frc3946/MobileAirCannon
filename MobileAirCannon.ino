#define WATCHDOG_TIMEOUT WDTO_500MS
#define XBEE_BAUD 115200
#define XBEE_TIMEOUT 150
#define BUTTON_UP_DELAY 500
#define BUTTON_DOWN_DELAY 1500
#define FIRING_TIME 30


#include "./SpeedController.h"
#include "./Spike.h"
#include "./Relay.h"
#include "./Controller.h"

#include <avr/wdt.h> //WatchDog
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
            relay.set(Relay::PWR); //open relay and reset timer
            timer = millis();
            firingState++;
          }
          break;

        case 4: //firing! wait for timeout and then close the relay
          if (millis() - timer > FIRING_TIME) {
           relay.set(Relay::GND); //If so, turn valve off
           firingState =0;
          }
          break;
      }
    }
    void attach(uint8_t pin) {
      relay.attach(pin);
      relay.set(Relay::GND);
    }
    void shutOff() { //Closes Valve
      relay.set(Relay::GND);
    }
};
unsigned long xbeeTimer;

Controller controller;

Talon leftMotor;
Talon rightMotor;

Barrel leftBarrel;
Barrel rightBarrel;

//PinMap

  uint8_t leftMotorPin = 2;
  uint8_t rightMotorPin = 5;
  uint8_t leftBarrelPin = 8;
  uint8_t rightBarrelPin = 9;
  
void setup() {
  Serial.begin(9600);
  //while(!Serial); //Wait for Serial Port to Connect. (For Leonardo Only)
  Serial1.begin(XBEE_BAUD);
  controller.attach(Serial1);

  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);

  leftBarrel.attach(leftBarrelPin);
  rightBarrel.attach(rightBarrelPin);

  birthWatchdog(WATCHDOG_TIMEOUT);
}

void loop() {
  feedWatchdog();

  if(controller.update()) { //Successful Data Retreval from Controller
    xbeeTimer = millis();
  //* Remove one of the "/" to comment out block
    controller.printFrameData(Serial);
    Serial.print("LY: ");
  	Serial.println(controller.getLY());
  	Serial.print("RY: ");
  	Serial.println(controller.getRY());
  	if(controller.getL1())
  		Serial.println("L1: True");
  	else
  		Serial.println("L1: False");
  	if(controller.getR1())
  		Serial.println("R1: True");
  	else
  		Serial.println("R1: False");
	//*/
	//* Remove one of the "/" to comment out block
    leftBarrel.updateFireButton(controller.getL1());
    rightBarrel.updateFireButton(controller.getR1());
	
    leftMotor.set(controller.getLY());
    rightMotor.set(controller.getRY());
	//*/
  } else {
    if(millis() - xbeeTimer < XBEE_TIMEOUT) { 
      leftBarrel.updateFireButton(controller.getL1());
      rightBarrel.updateFireButton(controller.getR1());
    } else {
      leftMotor.set(0);
      rightMotor.set(0);
      leftBarrel.shutOff();
      rightBarrel.shutOff();
    }
    //controller.printErrorMessage(Serial); //Handle Controller Error Codes
  }
}
