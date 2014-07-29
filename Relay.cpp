#include "Relay.h"

const Relay::Direction Relay::OFF(false);
const Relay::Direction Relay::ON(true);

//-------------------------------------------
Relay::Direction::Direction() { //default needed for whatever reason
	_state = false;
}

Relay::Direction::Direction(bool state) {
	_state = state;
}
bool Relay::Direction::get_state() {
	return _state;
}

bool Relay::Direction::compare(Relay::Direction direction) {
	if(direction.get_state() == _state)
		return true;
	return false;
}
//-------------------------------------------

Relay::Relay() {
	_init = false;
}

void Relay::attach(uint8_t pin) {
	_pin = pin;

	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	
	set(Relay::OFF);
	_init = true;
}

void Relay::set(Relay::Direction direction) {
	if(_init) {
		_direction = direction;
		digitalWrite(_pin, _direction.get_state());
	}
}

Relay::Direction Relay::get() {
	return _direction;
}
uint8_t Relay::get_pin() {
	return _pin;
}

void Relay::off() {
	set(Relay::OFF);
}
void Relay::on() {
	set(Relay::ON);
}