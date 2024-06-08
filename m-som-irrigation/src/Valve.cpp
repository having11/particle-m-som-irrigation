#include "Valve.h"

Valve::Valve(uint8_t pin)
  : _pin{pin} {
}

void Valve::init() {
  pinSetDriveStrength(_pin, DriveStrength::HIGH);
  pinMode(_pin, OUTPUT);
  _lastActiveTime = 0;
  setState(false);
}

void Valve::on() {
  setState(true);
}

void Valve::off() {
  setState(false);
}

bool Valve::active() const {
  return _active;
}

int32_t Valve::activeTime() const {
  return active() ? (millis() - _lastActiveTime) : -1;
}

void Valve::setState(bool state) {
  digitalWrite(_pin, state);
  _active = state;

  if (_active) {
    _lastActiveTime = millis();
  }
}