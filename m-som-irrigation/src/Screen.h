#pragma once

#include "Particle.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <memory>

#include "graphics/Image.h"

class Screen {
  public:
    Screen(uint8_t valveCount);
    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;
    ~Screen();

    void init();
    void setValve(uint8_t index, bool state);
    void setMoisture(uint8_t index, double value);
    void setConnectedStatus(bool state);
    void setFlowSensorStatus(bool state);

  private:
    void execute();
    void drawFan(const tImage* im);
    void drawValve(uint8_t index, const tImage* im);
    void drawConnected(const tImage* im);
    void drawMoisture(uint8_t index, double value);

    uint32_t _lastAnimated;
    uint8_t _valveCount;
    Thread *_thread;
    Mutex _mutex;
    volatile bool _flowSensorStatus;
    uint8_t _flowFanFrame = 0;
    std::unique_ptr<Adafruit_SSD1306> _display;
};