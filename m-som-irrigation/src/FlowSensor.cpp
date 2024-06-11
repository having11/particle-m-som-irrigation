#include "FlowSensor.h"

FlowSensor::FlowSensor(uint8_t pin, double flowrateCoeff) :
  _pin{pin}, _flowrateCoeff{flowrateCoeff}, _lastFlowRate{0.0}, _currentVolume{0.0} {
}

FlowSensor::~FlowSensor() {
  if (_thread) {
    delete _thread;
    _thread = nullptr;
  }
}

void FlowSensor::init() {
  pinMode(_pin, INPUT_PULLUP);
  attachInterrupt(_pin, &FlowSensor::flowTick, this, RISING);
  _ticks = 0;
}

double FlowSensor::flowRate() {
  double ret = _lastFlowRate;

  return ret * 60;
}

double FlowSensor::volume() {
  double ret = _currentVolume;

  return ret;
}

void FlowSensor::execute() {
  if (millis() - _dt >= 500) {
    _t0 = millis();
    _dt = millis();
    _lastFlowRate = _ticks / _flowrateCoeff;
    _currentVolume += (_lastFlowRate / 60) * ((millis() - _t0) / 1000.0);
    _ticks = 0;
  }
}

void FlowSensor::flowTick() {
  _ticks++;
}