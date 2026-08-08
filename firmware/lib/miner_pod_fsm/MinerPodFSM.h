#pragma once
#include <stdint.h>
#include "MinerPodState.h"

// Sensor snapshot the FSM reads each tick. Defaults are all "not ready" so
// the FSM never advances on uninitialized data.
struct MinerPodSensors {
  bool dispatchReceived = false;
  bool anchorConfirmed = false;

  // Containment seal is read from three independent sensors rather than
  // one. Losing dust containment on a real mining pod means uncontrolled
  // debris -- directly against the org's zero-dust mission (see
  // space-reclamation) -- so a single failed/miswired sensor should not
  // be able to gate drilling open by itself. See sealConfirmed() below.
  bool sealSensor1 = false;
  bool sealSensor2 = false;
  bool sealSensor3 = false;

  bool materialCollected = false;  // drilling phase complete
  bool crushComplete = false;
  bool refineComplete = false;
  bool transferConfirmed = false;

  bool faultDetected = false;
};

class MinerPodFSM {
 public:
  void begin(uint32_t nowMillis);
  void update(const MinerPodSensors& sensors, uint32_t nowMillis);

  MinerPodState state() const { return currentState; }
  uint32_t timeInState(uint32_t nowMillis) const { return nowMillis - lastStateChange; }

  // 2-of-3 voting: containment is only considered sealed if at least two
  // of the three independent seal sensors agree. A single dissenting
  // sensor (wiring fault, one bad sensor) does not block the pod, but a
  // single sensor reading "sealed" alone can't open the gate either.
  static bool sealConfirmed(const MinerPodSensors& s) {
    const uint8_t votes = (s.sealSensor1 ? 1 : 0) + (s.sealSensor2 ? 1 : 0) + (s.sealSensor3 ? 1 : 0);
    return votes >= 2;
  }

 private:
  void transitionTo(MinerPodState newState, uint32_t nowMillis);
  void enterState(MinerPodState state);
  void exitState(MinerPodState state);

  MinerPodState currentState = Idle;
  uint32_t lastStateChange = 0;
};
