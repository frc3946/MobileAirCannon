/*
||
|| @description
|| | Provides base for using common FRC speedcontrollers (Talon, Jaguar, Victor)
|| | Watchdog Safety Feature (if no update (set method) is called in .5 seconds, the motor is cut off)
|| | USAGE:
|| | SpeedController motor; //instance of class, Victor, Jaguar, Talon
|| | 	ie: Talon motor;
|| | motor.attach(pin);
|| | 	ie: motor.attach(13);
|| | motor.set(speed); //valid range -100 to 100
|| | 	ie: motor.set(50);
|| | 
|| #
||
|| @author
|| | Gustave Michel III
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU General Public
|| | License as published by the Free Software Foundation; version
|| | 4 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | General Public License for more details.
|| |
|| | You should have received a copy of the GNU General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
|| #
||
|| @version
|| | v1.0
|| | 07 July 2014
|| #
||
*/


#ifndef SPEEDCONTROLLER_h
#define SPEEDCONTROLLER_h

#include <inttypes.h>

/* 
 * Defines for 16 bit timers used with  SpeedController library 
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the curent board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 *
 */

// Say which 16 bit timers can be used and in what order
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define _useTimer5
#define _useTimer1 
#define _useTimer3
#define _useTimer4 
typedef enum { _timer5, _timer1, _timer3, _timer4, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_ATmega32U4__)  
#define _useTimer1 
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define _useTimer3
#define _useTimer1
typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t ;

#elif defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
#define _useTimer3
#define _useTimer1
typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t ;

#else  // everything else
#define _useTimer1
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t ;                  
#endif

#define SpeedController_VERSION           2      // software version of this library

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a speedController  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a speedController 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when speedController is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh speedControllers in microseconds 

#define SERVOS_PER_TIMER       12     // the maximum number of speedControllers controlled by one timer 
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid speedController index

#define MAX_RUNS 25 //incraments of 50 should be ~1 second

typedef struct  {
  uint8_t nbr        :6 ;             // a pin number from 0 to 63
  uint8_t isActive   :1 ;             // true if this channel is enabled, pin not pulsed if false 
} SpeedControllerPin_t;

typedef struct {
  SpeedControllerPin_t Pin;
  unsigned int ticks;
  uint8_t runs;
} speedController_t;

class SpeedController
{
public:
	SpeedController();
	uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
	uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes. 
	void detach();
	void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
	void writeMicroseconds(int value); // Write pulse width in microseconds 
	int read();                        // returns current pulse width as an angle between 0 and 180 degrees
	int readMicroseconds();            // returns current pulse width in microseconds for this speedController (was read_us() in first release)
	bool attached();                   // return true if this speedController is attached, otherwise false 
	int _min;
	int _max;
	int _speed;
	void set(int speed); //-100 to 100
	int get();
private:
	uint8_t speedControllerIndex;               // index into the channel data for this speedController
	int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH    
	int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH   
};

class Talon : SpeedController
{
public:
	Talon();
	uint8_t attach(int pin);
	void set(int speed); //-100 to 100
	int get();
private:
};

class Victor : SpeedController
{
public:
	Victor();
	uint8_t attach(int pin);
	void set(int speed); //-100 to 100
	int get();
private:
};

class Jaguar : SpeedController
{
public:
	Jaguar();
	uint8_t attach(int pin);
	void set(int speed); //-100 to 100
	int get();
private:
};

class Servo : SpeedController
{
public:
	Servo();
	uint8_t attach(int pin);
	void set(int speed); //-100 to 100
	int get();
private:
};

#endif