#include "FlowSensor.h"

FlowSensor::FlowSensor(uint8_t pin, double flowrateCoeff) :
  _lastFlowRate{0.0}, _currentVolume{0.0} {
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
  _thread = new Thread("FlowSensor", std::bind(&FlowSensor::execute, this), OS_THREAD_PRIORITY_DEFAULT, OS_THREAD_STACK_SIZE_DEFAULT);
}

double FlowSensor::flowRate() {
  _mutex.lock();
  double ret = _lastFlowRate;
  _mutex.unlock();

  return ret * 60;
}

double FlowSensor::volume() {
  _mutex.lock();
  double ret = _currentVolume;
  _mutex.unlock();

  return ret;
}

void FlowSensor::execute() {
  while (true) {
    _dt = millis() - _t0;

    if (_dt >= 500) {
      _t0 = millis();
      _mutex.lock();
      _lastFlowRate = _ticks / _flowrateCoeff;
      _currentVolume += (_lastFlowRate / 60) * (_dt / 1000.0);
      _mutex.unlock();
      _ticks = 0;
    } else {
      // yield to other threads
      delay(1);
    }
  }
}

void FlowSensor::flowTick() {
  _ticks++;
}