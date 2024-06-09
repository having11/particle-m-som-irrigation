#pragma once

#include "Particle.h"

constexpr uint8_t kValveCount = 4;
constexpr uint8_t kValve0Pin = D2;
constexpr uint8_t kValve1Pin = D3;
constexpr uint8_t kValve2Pin = D4;
constexpr uint8_t kValve3Pin = D5;
const uint8_t valvePins[kValveCount] = {kValve0Pin, kValve1Pin, kValve2Pin, kValve3Pin};

constexpr uint8_t kFlowSensorPin = D6;

constexpr uint8_t kSoilSensorCount = 2;
constexpr uint8_t kSoilSensor0Pin = A0;
constexpr uint8_t kSoilSensor1Pin = A1;
const uint8_t soilSensorPins[kSoilSensorCount] = {kSoilSensor0Pin, kSoilSensor1Pin};

constexpr uint16_t kAirMoistureValue = 1000;
constexpr uint16_t kWaterMoistureValue = 3000;

constexpr double kMinFlowRate = 1;

// 5 minutes
constexpr uint32_t kMaxValveOpenMs = 60 * 5 * 1000;
// 1 minute
constexpr uint32_t kLedgerUpdateRateMs = 60 * 1000;

enum class ValveState {
  OK = 1,
  OUT_OF_BOUNDS = -1,
  FAULT_NO_FLOW = -2,
};