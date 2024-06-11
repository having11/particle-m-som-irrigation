#include "Screen.h"
#include "graphics/check.h"
#include "graphics/fan-0.h"
#include "graphics/fan-1.h"
#include "graphics/fan-2.h"
#include "graphics/valve-inactive.h"
#include "graphics/valve.h"
#include "graphics/x.h"

constexpr uint8_t kScreenWidth = 128;
constexpr uint8_t kScreenHeight = 64;

Screen::Screen(uint8_t valveCount) : _valveCount{valveCount} {
  _display = std::make_unique<Adafruit_SSD1306>(kScreenWidth, kScreenHeight, &Wire, -1);
}

Screen::~Screen() {
  if (_thread) {
    delete _thread;
    _thread = nullptr;
  }
}

void Screen::init() {
  if (!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Log.error("Error initializing screen!");

    while (1) {
      delay(500);
    }
  }

  _display->clearDisplay();
  _display->setTextColor(WHITE);
  _display->setFont(NULL);
  _display->display();
}

void Screen::setValve(uint8_t index, bool state) {
  auto* im = state ? &valveBmp : &valveInactiveBmp;
  drawValve(index, im);
}

void Screen::setMoisture(uint8_t index, double value) {
  drawMoisture(index, value);
}

void Screen::setConnectedStatus(bool state) {
  auto* im = state ? &checkBmp : &xBmp;
  drawConnected(im);
}

void Screen::setFlowSensorStatus(bool state) {
  _flowSensorStatus = state;
}

void Screen::execute() {
  if (millis() - _lastAnimated >= 1000) {
    Log.info("screen");
    _lastAnimated = millis();

    if (_flowSensorStatus) {
      switch (_flowFanFrame) {
        case 0:
        case 5:
          // show fan 0
          drawFan(&fan0Bmp);
          break;
        case 1:
        case 4:
          // show fan 1
          drawFan(&fan1Bmp);
          break;
        case 2:
        case 3:
          // show fan 2
          drawFan(&fan2Bmp);
          break;
      }

      _flowFanFrame = (++_flowFanFrame) % 6;
    } else {
      _flowFanFrame = 0;
    }
  }
}

void Screen::drawFan(const tImage *im) {
  _display->fillRect(100, 60, im->width, im->height, 0x00);
  _display->drawBitmap(100, 60 - im->height, im->data, im->width, im->height, 0x1, 0x0);
  _display->display();
}

void Screen::drawValve(uint8_t index, const tImage *im) {
  uint8_t x = (im->width + 10) * index;

  _display->fillRect(x, 5, im->width, im->height, 0x00);
  _display->drawBitmap(x, 5, im->data, im->width, im->height, 0x1, 0x0);
  _display->display();
}

void Screen::drawConnected(const tImage *im) {
  _display->fillRect(100, 10, im->width, im->height, 0x00);
  _display->drawBitmap(100, 10, im->data, im->width, im->height, 0x1, 0x0);
  _display->display();
}

void Screen::drawMoisture(uint8_t index, double value) {
  uint8_t x = 24 * index + 40;
  uint8_t y = 48;

  _display->setTextSize(1);
  _display->setCursor(x, y);
  _display->fillRect(x, y, 32, 8, 0x00);
  _display->printf("%.0f%%", value);
  _display->display();
}
