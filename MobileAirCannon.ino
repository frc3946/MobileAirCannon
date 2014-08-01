#define WATCHDOG_TIMEOUT WDTO_500MS
#define XBEE_BAUD 115200

#define FIRE_DELAY 1500
#define RELAY_DELAY 30


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
    bool primed;
    bool fired;
    bool resetting;
    unsigned long timer; //used to track button and track relay
  public:
    Barrel() {
      primed = false;
      fired = false;
      resetting = false;
      timer = millis();
    }
    void check(bool button) {
      if(!resetting || (resetting && !button)) { //are we reset, or resettings
        resetting = false; //if so, make sure we are reset!
        if(!primed) { //Has a Hold already been Established
          if(button) {//If not, are we currently trying to
            if(millis() - timer > FIRE_DELAY) {//if so has it been
              primed = true;
            }
          }else //reset timer until hold is trying to be established
            timer = millis();
        }else { //Hold established, now firing
          if(!fired) { //Has valve been opened
            Serial.println("FIRED!");
            relay.set(Relay::GND); //if not, open it
            timer = millis(); //reset timer to track time open
            fired = true; //and notify that it has been opened
          }else { //Valve has opened
            if(millis() - timer > RELAY_DELAY) { //Has the alotted open time crossed
              relay.set(Relay::PWR); //If so, turn valve off
              primed = false; //reset values and timer for establishing a new hold
              fired = false;
              resetting = true; //to force user to depress button before firing again
              timer = millis();
            }
          }
        }
      }
    }
    void attach(uint8_t pin) {
      relay.attach(pin);
      relay.set(Relay::PWR);
    }
};

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

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);

  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);

  leftBarrel.attach(leftBarrelPin);
  rightBarrel.attach(rightBarrelPin);

  birthWatchdog(WATCHDOG_TIMEOUT);
}

void loop() {
  feedWatchdog();

  if(controller.update()) { //Successful Data Retreval from Controller
  //*
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
	//*
    leftBarrel.check(controller.getL1());
    rightBarrel.check(controller.getR1());
	
    leftMotor.set(controller.getLY());
    rightMotor.set(controller.getRY());
	//*/
  } else { //Handle Controller Error Codes
    leftBarrel.check(controller.getL1());
    rightBarrel.check(controller.getR1());
    //controller.printErrorMessage(Serial);
  }
}
