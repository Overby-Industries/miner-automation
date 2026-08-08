# Miner Automation

[![Build & Test](https://github.com/Overby-Industries/miner-automation/actions/workflows/build.yml/badge.svg)](https://github.com/Overby-Industries/miner-automation/actions/workflows/build.yml)

**Overby Industries — automation for the full ISRU stack: extraction,
refining, advanced manufacturing, and food production.**

Not just "drills and crushers." A self-sufficient space-based civilization
needs to extract materials, refine them, manufacture precision components
from them, and grow food — all automated, none of it ladder logic. This
repo is the shared home for that automation, from the mining pod prototype
running today through the industrial-scale processing this mission is
ultimately built around.

See [`ROADMAP.md`](ROADMAP.md) for the phased plan.

## What's Actually Built

- **`MinerPodFSM`** — a finite-state machine for a single mining/
  containment pod (Idle → Anchoring → ContainmentDeploy → Drilling →
  Crushing → Refining → Transfer), with 2-of-3 redundant voting on
  containment-seal sensors before drilling is allowed to start, and a
  fault state that preempts everything and requires deliberate recovery.
  Plain, portable C++ — **no ladder logic, no PLC required at this stage.**
- Builds and runs on **both an Arduino Uno and an ESP32** from the same
  source. Verified: `pio run -e uno` and `pio run -e esp32` both succeed;
  `pio test -e native` passes the full logic suite (happy path, seal
  voting, fault preemption) on desktop, no hardware needed.

## Repo Structure

```
miner-automation/
├── ROADMAP.md
├── firmware/
│   ├── lib/miner_pod_fsm/      # MinerPodFSM -- portable C++, no Arduino dependency
│   ├── src/miner_pod.ino       # Arduino/ESP32 entry point (board-specific watchdog only)
│   └── test/test_fsm/          # Unity-based logic tests, run via `pio test -e native`
├── docs/
│   ├── hardware-decision.md            # why C++/microcontrollers now, PLCnext/TwinCAT later
│   ├── pod-fsm-draft.md                # extraction/containment FSM design notes
│   ├── refining-automation-draft.md    # smelting, volatile extraction, aggregate production
│   ├── advanced-manufacturing-draft.md # crystal growth and other precision ISRU manufacturing
│   └── space-agriculture-draft.md      # automated grow-module control
└── platformio.ini
```

## Getting Started

```bash
pip install platformio
pio run -e uno              # build for an Arduino Uno
pio run -e esp32             # build for an ESP32
pio test -e native           # run the FSM logic tests on your desktop, no hardware needed
pio run -e uno -t upload     # flash to a connected Uno
```

## Hardware Path

**Now:** plain C++ on an Arduino Uno and an ESP32 (both already on hand).
**Later:** [PLCnext Technology](https://www.phoenixcontact.com/) and/or
[Beckhoff TwinCAT 3](https://www.beckhoff.com/) once real industrial
resource processors exist for refining and advanced manufacturing — both
let you write real C++ instead of ladder logic on certified industrial
hardware. Full reasoning in [`docs/hardware-decision.md`](docs/hardware-decision.md).

## Relationship to Other Repos

- **[`space-reclamation`](https://github.com/Overby-Industries/space-reclamation)** —
  this repo builds the PLC/automation logic that repo's README calls for,
  shared between mining and reclamation pods.
- **[`power-and-propulsion`](https://github.com/Overby-Industries/power-and-propulsion)** —
  `ShuttleFSM` is the pattern `MinerPodFSM` follows (non-blocking,
  watchdog-guarded, explicit fault state).
- **[`telemetry-protocol`](https://github.com/Overby-Industries/telemetry-protocol)** —
  OTF-1 is how a pod reports processing events upward.
- **[`starlifter-os`](https://github.com/Overby-Industries/starlifter-os)** —
  the fleet coordinator multiple pods would eventually report to.
- **[`outpost-core`](https://github.com/Overby-Industries/outpost-core)** —
  shares an atmosphere-system boundary with the space-agriculture work
  here once both exist.

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) and [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md).
Embedded/controls engineers, PLC programmers (PLCnext/TwinCAT experience
especially useful for Phase 3+), materials scientists (crystal growth),
and controlled-environment agriculture folks are all useful here.

## License

[Apache 2.0](LICENSE).
