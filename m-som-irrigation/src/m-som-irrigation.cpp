/* 
 * Project myProject
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

void setup() {
  Particle.function("activateValve", activateValve);
  Particle.function("shutoffValve", shutoffValve);

  ledger = Particle.ledger("smartIrrigation");

  for (uint8_t i = 0; i < kValveCount; i++) {
    soilSensors[i] = std::make_unique<SoilSensor>(soilSensorPins[i],
      kAirMoistureValue, kWaterMoistureValue);
    valves[i] = std::make_unique<Valve>(valvePins[i]);

    soilSensors[i]->init();
    valves[i]->init();
  }

  flowSensor.init();
  screen.init();
}

void loop() {
  if (millis() - lastUpdateMs >= kLedgerUpdateRateMs) {
    lastUpdateMs = millis();

    for (auto i = 0; i < kValveCount; i++) {
      if (valves[i]->activeTime() >= kMaxValveOpenMs) {
        shutoffValve(String(i));
      }

      ledger.get()["valves"][i].set("state", valves[i]->active());
    }

    VariantArray soilSensorData;
    for (auto i = 0; i < kSoilSensorCount; i++) {
      Variant soilData;
      soilData.set("percentage", soilSensors[i]->moisturePercentage());
      soilData.set("raw", soilSensors[i]->rawValue());
      soilSensorData.append(soilData);
    }

    ledger.get()["soilSensors"] = soilSensorData;

    Variant flowData;
    flowData.set("rate", flowSensor.flowRate());
    flowData.set("vol", flowSensor.volume());
    ledger.get()["flow"] = flowData;

    screen.setConnectedStatus(Particle.connected());
    screen.setFlowSensorStatus(flowSensor.flowRate() >= kMinFlowRate);
  }
}

int activateValve(String index) {
  // Parse to int
  int valveIndex = index.toInt();
  if (valveIndex < 0 || valveIndex >= kValveCount) {
    return static_cast<int>(ValveState::OUT_OF_BOUNDS);
  }

  auto* valve = valves[valveIndex].get();

  // Check if valve is already active
  if (valve->active()) {
    return static_cast<int>(ValveState::OK);
  }

  // Check if water is present and can flow
  if (valves[0]->checkFlow(flowSensor, kMinFlowRate)) {
    valve->on();
    screen.setValve(valveIndex, true);
    ledger.get()["valves"][valveIndex].set("state", true);
    ledger.get()["valves"][valveIndex].set("ts", Time.timeStr());

    return static_cast<int>(ValveState::OK);
  }

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
    ledger.get()["valves"][valveIndex].set("state", false);
    ledger.get()["valves"][valveIndex].set("ts", Time.timeStr());
  }

  return static_cast<int>(ValveState::OK);
}