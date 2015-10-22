/*
||
|| @description
|| | Control a Relay
|| | Connection: Pin
|| | USAGE:
|| | Relay relay; //instance of class
|| | relay.attach(#); //# is a digital pin
|| | 	ie: relay.attach(8);
|| | relay.set(Relay::DIRECTION); //valid DIRECTION OFF, ON
|| | 	ie: relay.set(Relay::ON);
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
|| | 14 July 2014
|| #
||
*/

#ifndef RELAY_H
#define RELAY_H


#include "Arduino.h"

class Relay {
private:
	//Defines potential states for the Relay
	class Direction {
	private:
		bool _state;
	public:
		Direction();
		Direction(bool state);
		bool get_state();
		bool compare(Direction direction);
	};

	bool _init; //confirm object properly initiated in code
	Direction _direction; //abstract state object
	uint8_t _pin;
public:

	static const Direction OFF; //false
	static const Direction GND; //false
	static const Direction ON; //true
	static const Direction PWR; //true

	Relay();
	void attach(uint8_t pin);
	void set(Direction direction);
	Direction get();
	uint8_t get_pin();

	void off();
	void on();
};
#endif
