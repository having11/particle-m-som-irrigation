/* 
 * Project M-SoM Smart Irrigation
 * Author: Evan Rust
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */


#include "Particle.h"

#include <memory>

#include "Constants.h"
#include "FlowSensor.h"
#include "Screen.h"
#include "SoilSensor.h"
#include "Valve.h"

SYSTEM_MODE(AUTOMATIC);

SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

FlowSensor flowSensor{kFlowSensorPin};
Screen screen{kValveCount};
std::unique_ptr<SoilSensor> soilSensors[kSoilSensorCount];
std::unique_ptr<Valve> valves[kValveCount];
Ledger ledger;

static uint32_t lastUpdateMs = 0;

int activateValve(String);
int shutoffValve(String index);
void update(void);

void setup() {
  Particle.function("activateValve", activateValve);
  Particle.function("shutoffValve", shutoffValve);

  ledger = Particle.ledger("smart-irrigation");
  Variant resetData;
  Variant flowData;
  VariantArray sensorArray;
  VariantArray valveArray;

  for (uint8_t i = 0; i < kValveCount; i++) {
    valves[i] = std::make_unique<Valve>(valvePins[i]);
    valves[i]->init();
    Variant valveData;
    valveData.set("state", false);
    valveArray.append(valveData);
  }

  for (uint8_t i = 0; i < kSoilSensorCount; i++) {
    soilSensors[i] = std::make_unique<SoilSensor>(soilSensorPins[i],
      kAirMoistureValue, kWaterMoistureValue);
    soilSensors[i]->init();
    Variant sensorData;
    sensorData.set("percentage", 0.0);
    sensorData.set("raw", 0.0);
    sensorArray.append(sensorData);
  }

  flowData.set("rate", 0.0);
  flowData.set("vol", 0.0);

  resetData.set("valves", valveArray);
  resetData.set("soilSensors", sensorArray);
  resetData.set("flow", flowData);
  ledger.set(resetData);

  flowSensor.init();
  screen.init();

  for (auto i = 0; i < kValveCount; i++) {
    screen.setValve(i, false);
  }

  update();
}

void loop() {
  if (millis() - lastUpdateMs >= kLedgerUpdateRateMs) {
    lastUpdateMs = millis();

    update();
  }

  screen.execute();
  flowSensor.execute();
}

int activateValve(String index) {
  Log.info("-1");
  // Parse to int
  int valveIndex = index.toInt();
  if (valveIndex < 0 || valveIndex >= kValveCount) {
    return static_cast<int>(ValveState::OUT_OF_BOUNDS);
  }

  Log.info("0");

  auto* valve = valves[valveIndex].get();

  // Check if valve is already active
  if (valve->active()) {
    return static_cast<int>(ValveState::OK);
  }

  Log.info("1");

  // Check if water is present and can flow
  // if (!valves[0]->checkFlow(flowSensor, kMinFlowRate)) {
  //   valve->on();
  //   screen.setValve(valveIndex, true);

  //   Log.info("2");

  //   return static_cast<int>(ValveState::OK);
  // }

  valve->on();
  screen.setValve(valveIndex, true);

  return static_cast<int>(ValveState::OK);

  Log.info("3");
  return static_cast<int>(ValveState::FAULT_NO_FLOW);
}

int shutoffValve(String index) {
  // Parse to int
  int valveIndex = index.toInt();
  if (valveIndex < 0 || valveIndex >= kValveCount) {
    return static_cast<int>(ValveState::OUT_OF_BOUNDS);
  }

  auto* valve = valves[valveIndex].get();

  if (valve->active()) {
    valve->off();
    screen.setValve(valveIndex, false);
  }

  return static_cast<int>(ValveState::OK);
}

void update() {
  Log.info("UPDATE %lu", millis());
  Variant data;

  VariantArray valveArr;
  for (auto i = 0; i < kValveCount; i++) {
    Variant valveData;
    if (valves[i]->activeTime() >= kMaxValveOpenMs) {
      shutoffValve(String(i));
    }

    valveData.set("state", valves[i]->active());
    valveArr.append(valveData);
  }

  VariantArray soilSensorData;
  for (auto i = 0; i < kSoilSensorCount; i++) {
    Variant soilData;
    soilData.set("percentage", soilSensors[i]->moisturePercentage());
    soilData.set("raw", soilSensors[i]->rawValue());
    soilSensorData.append(soilData);
    screen.setMoisture(i, soilSensors[i]->moisturePercentage());
  }

  Variant flowData;
  flowData.set("rate", flowSensor.flowRate());
  flowData.set("vol", flowSensor.volume());

  data.set("valves", valveArr);
  data.set("soilSensors", soilSensorData);
  data.set("flow", flowData);
  ledger.set(data, particle::Ledger::SetMode::REPLACE);

  screen.setConnectedStatus(Particle.connected());
  screen.setFlowSensorStatus(flowSensor.flowRate() >= kMinFlowRate);
}