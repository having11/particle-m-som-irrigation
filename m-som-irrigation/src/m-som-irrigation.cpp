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

void setup() {
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
  // Every minute, update screen's connected status

  // Every 5 minutes, read the soil moisture sensors
    // Update the screen
    // Update Ledger
  // Every 5 minutes, get the current flowrate
    // Update Ledger
}

int activateValve(String index) {
  // Parse to int

  // Check if valve is already active
    // If it is, don't do anything
    // Otherwise:
      // Check if enough time has elapsed since last activation
        // If not enough delay, return 2
      // Activate + update screen
        // Also activate the first valve if not already going
        // and ensure it has flow
      // Store timestamp in Ledger
      // Return 1
}
