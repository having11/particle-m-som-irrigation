#pragma once

#include "Particle.h"

class Valve {
  public:
    Valve(uint8_t pin);

    void init();
    void on();
    void off();
    bool active() const;

    /**
     * @brief The number of ms the valve has been on
     * 
     * @remark Will be -1 if state is off
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