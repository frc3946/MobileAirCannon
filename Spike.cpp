#include "Spike.h"

const Spike::Channel Spike::CHANNEL1(2,3);
const Spike::Channel Spike::CHANNEL2(4,5);
const Spike::Channel Spike::CHANNEL3(6,7);
const Spike::Channel Spike::CHANNEL4(8,9);
const Spike::Channel Spike::CHANNEL5(10,11);
const Spike::Channel Spike::CHANNEL6(12,13);

const Spike::Direction Spike::OFF(false, false);
const Spike::Direction Spike::FORWARD(true, false);
const Spike::Direction Spike::REVERSE(false, true);
const Spike::Direction Spike::ON(true, true);

//-------------------------------------------
Spike::Direction::Direction() { //default needed for whatever reason
	_a = false;
	_b = false;
}

Spike::Direction::Direction(bool a, bool b) {
	_a = a;
	_b = b;
}
bool Spike::Direction::get_a() {
	return _a;
}
bool Spike::Direction::get_b() {
	return _b;
}

bool Spike::Direction::compare(Spike::Direction direction) {
	if(direction.get_a() == _a && direction.get_b() == _b)
		return true;
	return false;
}
//-------------------------------------------
Spike::Channel::Channel() { //default needed for whatever reason
	_a = 0;
	_b = 0;
}

Spike::Channel::Channel(uint8_t a, uint8_t b) {
	_a = a;
	_b = b;
}
uint8_t Spike::Channel::get_a() {
	return _a;
}
uint8_t Spike::Channel::get_b() {
	return _b;
}
//-------------------------------------------

Spike::Spike() {
	_init = false;
}

void Spike::attach(Spike::Channel channel) {
	_channel = channel;

	pinMode(_channel.get_a(), OUTPUT);
	pinMode(_channel.get_b(), OUTPUT);
	digitalWrite(_channel.get_a(), LOW);
	digitalWrite(_channel.get_b(), LOW);
	
	set(Spike::OFF);
	_init = true;
}
void Spike::attach(uint8_t a, uint8_t b) {
	attach(Channel(a,b));
}

void Spike::set(Spike::Direction direction) {
	if(_init) {
		_direction = direction;
		digitalWrite(_channel.get_a(), _direction.get_a());
		digitalWrite(_channel.get_b(), _direction.get_b());
	}
}

Spike::Direction Spike::get() {
	return _direction;
}
Spike::Channel Spike::get_channel() {
	return _channel;
}

void Spike::off() {
	set(Spike::OFF);
}
void Spike::forward() {
	set(Spike::FORWARD);
}
void Spike::reverse() {
	set(Spike::REVERSE);
}