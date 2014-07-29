/*
||
|| @description
|| | Control an Innovation First/VEX Spike Relay
|| | Connection: A B Gnd
|| | USAGE:
|| | Spike spike; //instance of class
|| | spike.attach(Spike::CHANNEL#); //valid # 1-6
|| | 	ie: spike.attach(Spike::CHANNEL2);
|| | spike.set(Spike::DIRECTION); //valid DIRECTION OFF, FORWARD, REVERSE, ON
|| | 	ie: spike.set(Spike::FORWARD);
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
|| | v1.1
|| | 02 July 2014
|| #
||
*/

#ifndef SPIKE_H
#define SPIKE_H


#include "Arduino.h"

class Spike {
private:
	//Defines potential states for the Spike
	class Direction {
	private:
		bool _a;
		bool _b;
	public:
		Direction();
		Direction(bool a, bool b);
		bool get_a();
		bool get_b();
		bool compare(Direction direction);
	};
	//Defines potential pins for the Spike
	class Channel {
	private:
		uint8_t _a;
		uint8_t _b;
	public:
		Channel();
		Channel(uint8_t a, uint8_t b);
		uint8_t get_a();
		uint8_t get_b();
	};

	bool _init; //confirm object properly initiated in code
	Direction _direction; //abstract state object
	Channel _channel; //abstract port object
public:
	static const Channel CHANNEL1; //2,3
	static const Channel CHANNEL2; //4,5
	static const Channel CHANNEL3; //6,7
	static const Channel CHANNEL4; //8,9
	static const Channel CHANNEL5; //10,11
	static const Channel CHANNEL6; //12,13

	static const Direction OFF; //false, false
	static const Direction FORWARD; //true, false
	static const Direction REVERSE; //false, true
	static const Direction ON; //true, true

	Spike();
	void attach(Channel channel);
	void attach(uint8_t a, uint8_t b);
	void set(Direction direction);
	Direction get();
	Channel get_channel();

	void off();
	void forward();
	void reverse();
};
#endif
