#pragma once

#include "Particle.h"

#include "FlowSensor.h"

class Valve {
  public:
    Valve(uint8_t pin);

    void init();
    void on();
    void off();
    bool active() const;
    bool checkFlow(FlowSensor& sensor, double threshold);

    /**
     * @brief The number of ms the valve has been on
     * 
     * @remark Will be 0 if state is off
     * 
     * @return int32_t 
     */
    int32_t activeTime() const;

  private:
    void setState(bool state);

    uint8_t _pin;
    bool _active;
    uint32_t _lastActiveTime;
};