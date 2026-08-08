#include "MinerPodFSM.h"

#if defined(ARDUINO)
#include <Arduino.h>
#define MPF_LOG(msg) Serial.println(msg)
#else
#include <cstdio>
#define MPF_LOG(msg) std::puts(msg)
#endif

void MinerPodFSM::begin(uint32_t nowMillis) {
  currentState = Idle;
  lastStateChange = nowMillis;
  enterState(Idle);
}

void MinerPodFSM::update(const MinerPodSensors& sensors, uint32_t nowMillis) {
  // Same philosophy as ShuttleFSM: a fault preempts every state, and
  // recovery is a deliberate, ground/operator-commanded action, not
  // automatic.
  if (sensors.faultDetected && currentState != Fault) {
    transitionTo(Fault, nowMillis);
    return;
  }

  switch (currentState) {
    case Idle:
      if (sensors.dispatchReceived) {
        transitionTo(Anchoring, nowMillis);
      }
      break;

    case Anchoring:
      if (sensors.anchorConfirmed) {
        transitionTo(ContainmentDeploy, nowMillis);
      }
      break;

    case ContainmentDeploy:
      // Gated on 2-of-3 sensor voting, not a single sensor -- see
      // sealConfirmed()'s comment in the header for why.
      if (sealConfirmed(sensors)) {
        transitionTo(Drilling, nowMillis);
      }
      break;

    case Drilling:
      if (sensors.materialCollected) {
        transitionTo(Crushing, nowMillis);
      }
      break;

    case Crushing:
      if (sensors.crushComplete) {
        transitionTo(Refining, nowMillis);
      }
      break;

    case Refining:
      if (sensors.refineComplete) {
        transitionTo(Transfer, nowMillis);
      }
      break;

    case Transfer:
      if (sensors.transferConfirmed) {
        transitionTo(Idle, nowMillis);
      }
      break;

    case Fault:
      // No automatic exit -- operator-commanded reset only.
      break;
  }
}

void MinerPodFSM::transitionTo(MinerPodState newState, uint32_t nowMillis) {
  exitState(currentState);
  currentState = newState;
  lastStateChange = nowMillis;
  enterState(currentState);
  MPF_LOG(minerPodStateName(currentState));
}

void MinerPodFSM::enterState(MinerPodState state) {
  switch (state) {
    case Idle:
      MPF_LOG("Entered Idle - awaiting dispatch");
      break;
    case ContainmentDeploy:
      MPF_LOG("Entered ContainmentDeploy - awaiting 2-of-3 seal confirmation");
      break;
    case Fault:
      MPF_LOG("FAULT - pod safed, awaiting operator reset");
      break;
    default:
      break;
  }
}

void MinerPodFSM::exitState(MinerPodState state) {
  (void)state;
}
