#define WATCHDOG_TIMEOUT WDTO_500MS
#define XBEE_BAUD 115200
#define FIRE_DELAY 1500
#define RELAY_DELAY 30

#include "SpeedController.h"
#include "Spike.h"
#include "Relay.h"
#include "Controller.h"

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
    unsigned long timer; //used to track button and track relay
  public:
    Barrel() {
      primed = false;
      fired = false;
      timer = millis();
    }
    void check(bool button) {
      if(!primed) { //Has a Hold already been Established
        if(button) //If not, are we currently trying to
          if(millis() - timer > FIRE_DELAY) //if so has it been
            primed = true;
        else //reset timer until hold is trying to be established
          timer = millis();
      }else { //Hold established, now firing
        if(!fired) { //Has valve been opened
          relay.set(Relay::ON); //if not, open it
          timer = millis(); //reset timer to track time open
          fired = true; //and notify that it has been opened
        } else { //Valve has opened
          if(millis() - timer > RELAY_DELAY) { //Has the alotted open time crossed
            relay.set(Relay::OFF); //If so, turn valve off
            primed = false; //reset values and timer for establishing a new hold
            fired = false;
            timer = millis();
          }
        }
      }
    }
    void attach(uint8_t pin) {
      relay.attach(pin);
    }
};

struct PowerPins {
  PowerPins(uint8_t pwr, uint8_t gnd) {
    pinMode(pwr, OUTPUT);
    pinMode(gnd, OUTPUT);
    digitalWrite(pwr, HIGH);
    digitalWrite(gnd, LOW);
  }
};

Controller controller;

Talon leftMotor;
Talon rightMotor;

Barrel leftBarrel;
Barrel rightBarrel;

//PinMap
  PowerPins leftPP(3,4);
  PowerPins rightPP(6,7);

  uint8_t leftMotorPin = 2;
  uint8_t rightMotorPin = 5;
  uint8_t leftBarrelPin = 8;
  uint8_t rightBarrelPin = 9;
  
void setup() {
  Serial.begin(9600);
  while(!Serial); //Wait for Serial Port to Connect. (For Leonardo Only)

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

    leftBarrel.check(controller.getL1());
    rightBarrel.check(controller.getR1());

    leftMotor.set(controller.getLY());
    rightMotor.set(controller.getRY());

  } else //Handle Controller Error Codes
    controller.printErrorMessage(Serial);

}
