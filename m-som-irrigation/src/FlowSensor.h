#pragma once

#include "Particle.h"

class FlowSensor {
  public:
    FlowSensor(uint8_t pin, double flowrateCoeff = 7.5);
    FlowSensor(const FlowSensor&) = delete;
    FlowSensor& operator=(const FlowSensor&) = delete;
    ~FlowSensor();

    void init();
    /**
     * @brief Get the current flow rate in L/hr
     * 
     * @return double 
     */
    double flowRate();
    /**
     * @brief Get the current volume in L since init()
     * 
     * @return double 
     */
    double volume();

  private:
    void execute();
    void flowTick();

    uint8_t _pin;
    long _dt, _t0;
    double _flowrateCoeff;
    double _lastFlowRate, _currentVolume;
    volatile uint32_t _ticks = 0;
    Thread *_thread = nullptr;
    Mutex _mutex;
};