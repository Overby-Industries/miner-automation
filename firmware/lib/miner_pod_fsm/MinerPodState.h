#pragma once
#include <stdint.h>

// Overby Industries miner/refinery pod -- top-level automation state
// machine. Mirrors the pattern established in power-and-propulsion's
// ShuttleFSM: fixed numeric values for stable telemetry (OTF-1), a fault
// state that preempts everything, and no automatic recovery out of it.
enum MinerPodState : uint8_t {
  Idle = 0,
  Anchoring = 1,
  ContainmentDeploy = 2,
  Drilling = 3,
  Crushing = 4,
  Refining = 5,
  Transfer = 6,
  Fault = 7
};

inline const char* minerPodStateName(MinerPodState s) {
  switch (s) {
    case Idle: return "Idle";
    case Anchoring: return "Anchoring";
    case ContainmentDeploy: return "ContainmentDeploy";
    case Drilling: return "Drilling";
    case Crushing: return "Crushing";
    case Refining: return "Refining";
    case Transfer: return "Transfer";
    case Fault: return "Fault";
    default: return "Unknown";
  }
}
