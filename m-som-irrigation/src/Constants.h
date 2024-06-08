#pragma once

#include "Particle.h"

constexpr uint8_t kValveCount = 4;
constexpr uint8_t kValve0Pin = D2;
constexpr uint8_t kValve1Pin = D3;
constexpr uint8_t kValve2Pin = D4;
constexpr uint8_t kValve3Pin = D5;
const uint8_t valvePins[kValveCount] = {kValve0Pin, kValve1Pin, kValve2Pin, kValve3Pin};

constexpr uint8_t kFlowSensorPin = D6;

constexpr uint8_t kSoilSensorCount = 4;
constexpr uint8_t kSoilSensor0Pin = A0;
constexpr uint8_t kSoilSensor1Pin = A1;
constexpr uint8_t kSoilSensor2Pin = A2;
constexpr uint8_t kSoilSensor3Pin = A3;
const uint8_t soilSensorPins[kSoilSensorCount] = {kSoilSensor0Pin, kSoilSensor1Pin, kSoilSensor2Pin, kSoilSensor3Pin};

constexpr uint16_t kAirMoistureValue = 1000;
constexpr uint16_t kWaterMoistureValue = 3000;

// 5 minutes
constexpr uint32_t kMaxValveOpenMs = 60 * 5 * 1000;