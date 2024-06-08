#pragma once

#include "Particle.h"

class SoilSensor {
  public:
    SoilSensor(uint8_t pin, uint16_t airCalibrationValue, uint16_t waterCalibrationValue);

    void init();
    double moisturePercentage();
    uint16_t rawValue();

  private:
    uint8_t _pin;
    uint16_t _airCalib, _waterCalib;
};