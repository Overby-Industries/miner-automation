// =============================================
// Overby Industries Miner/Refinery Pod FSM -- Arduino sketch
// Builds for both an Arduino Uno and an ESP32 (see platformio.ini) from
// the same source, using the portable MinerPodFSM in
// firmware/lib/miner_pod_fsm.
//
// Non-blocking: no delay() in loop(). Watchdog setup is board-specific
// (AVR vs. ESP32 use different APIs), so it's the one part of this file
// that's conditionally compiled.
// =============================================
#include <Arduino.h>
#include "MinerPodState.h"
#include "MinerPodFSM.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/wdt.h>
#define WDT_SETUP() wdt_enable(WDTO_8S)
#define WDT_KICK() wdt_reset()
#elif defined(ARDUINO_ARCH_ESP32)
#include <esp_task_wdt.h>
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
// Arduino-ESP32 3.x (ESP-IDF 5) replaced esp_task_wdt_init(timeout_s, panic)
// with one that takes a config struct, and the core has already initialised
// the task watchdog before setup() runs, so reconfigure it instead. Same
// settings as the old call: an 8 s timeout that panics on expiry; the idle
// mask keeps the core's default of watching core 0's idle task only.
static void esp32WdtSetup() {
  esp_task_wdt_config_t cfg;
  cfg.timeout_ms = 8000;
  cfg.idle_core_mask = 1;
  cfg.trigger_panic = true;
  esp_task_wdt_reconfigure(&cfg);
}
#define WDT_SETUP() esp32WdtSetup()
#else
#define WDT_SETUP() esp_task_wdt_init(8, true)
#endif
// Note (unchanged behaviour): loop() is not subscribed to the task watchdog
// (that takes esp_task_wdt_add(NULL), or enableLoopWDT() in the Arduino core),
// so on ESP32 this reset is a no-op and only the timeout above is configured.
#define WDT_KICK() esp_task_wdt_reset()
#else
#define WDT_SETUP()
#define WDT_KICK()
#endif

const uint8_t kDispatchPin = 2;   // ground-test stand-in for a real dispatch signal
const uint8_t kAnchorPin = 3;
const uint8_t kSealSensor1Pin = 4;
const uint8_t kSealSensor2Pin = 5;
const uint8_t kSealSensor3Pin = 6;
const uint8_t kFaultPin = 7;
const uint8_t kStatusLedPin = 13;

MinerPodFSM fsm;

void setup() {
  Serial.begin(115200);
  pinMode(kDispatchPin, INPUT_PULLUP);
  pinMode(kAnchorPin, INPUT_PULLUP);
  pinMode(kSealSensor1Pin, INPUT_PULLUP);
  pinMode(kSealSensor2Pin, INPUT_PULLUP);
  pinMode(kSealSensor3Pin, INPUT_PULLUP);
  pinMode(kFaultPin, INPUT_PULLUP);
  pinMode(kStatusLedPin, OUTPUT);

  Serial.println(F("Overby Industries Miner Pod FSM starting..."));
  fsm.begin(millis());
  WDT_SETUP();
}

void loop() {
  WDT_KICK();
  const uint32_t now = millis();

  // Replace each digitalRead() with a real sensor/process signal as
  // hardware comes online -- these pins are ground-test stand-ins.
  MinerPodSensors sensors;
  sensors.dispatchReceived = (digitalRead(kDispatchPin) == LOW);
  sensors.anchorConfirmed = (digitalRead(kAnchorPin) == LOW);
  sensors.sealSensor1 = (digitalRead(kSealSensor1Pin) == LOW);
  sensors.sealSensor2 = (digitalRead(kSealSensor2Pin) == LOW);
  sensors.sealSensor3 = (digitalRead(kSealSensor3Pin) == LOW);
  sensors.faultDetected = (digitalRead(kFaultPin) == LOW);
  // TODO once instrumented: materialCollected, crushComplete,
  // refineComplete, transferConfirmed.

  fsm.update(sensors, now);
  digitalWrite(kStatusLedPin, fsm.state() == Fault ? HIGH : LOW);
}
