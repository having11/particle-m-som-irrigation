#include "SoilSensor.h"

SoilSensor::SoilSensor(uint8_t pin, uint16_t airCalibrationValue, uint16_t waterCalibrationValue) :
  _pin{pin}, _airCalib{airCalibrationValue}, _waterCalib{waterCalibrationValue} {

}

void SoilSensor::init() {
}

double SoilSensor::moisturePercentage() {
  double percentage = map(rawValue(), _airCalib, _waterCalib, 0, 100);

  return std::clamp<double>(percentage, 0, 100);
}

uint16_t SoilSensor::rawValue() {
  return analogRead(_pin);
}