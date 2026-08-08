# Roadmap

**"Full ISRU means more than mining rocks — it means feeding, building, and
refining everything a space-based civilization needs, on-site."**

This roadmap covers the whole automation stack this repo is responsible
for: extraction, refining, advanced manufacturing, and food production.
Phases are tied to the [org-level roadmap](https://github.com/Overby-Industries/Overby-Industries#roadmap-high-level)'s
phases where there's a real dependency (e.g. refining automation doesn't
matter until there's something to refine), not to independent dates.

---

## Phase 1 — Mining Pod Prototype (in progress)

**Objective:** A working, tested single-pod extraction/containment FSM on
hardware already owned.

- [x] `MinerPodFSM` implemented in portable C++ (
      [`firmware/lib/miner_pod_fsm`](firmware/lib/miner_pod_fsm))
- [x] Builds and runs on both an Arduino Uno and an ESP32 from the same
      source (`pio run -e uno`, `pio run -e esp32`)
- [x] Native logic tests (`pio test -e native`) covering the happy path,
      2-of-3 containment-seal voting, and fault preemption
- [ ] Real sensor wiring (currently ground-test stand-ins on digital pins
      — see `firmware/src/miner_pod.ino`)
- [ ] OTF-1 telemetry output wired up (see
      [`telemetry-protocol`](https://github.com/Overby-Industries/telemetry-protocol))

**Corresponds to:** org roadmap Phase I (Foundations) — embedded systems
in C++, documented as they're built.

## Phase 2 — Networked Pods

**Objective:** Multiple ESP32-based pods coordinating, not just one pod
running standalone.

- [ ] WiFi/mesh telemetry reporting per pod (OTF-1 packets)
- [ ] Integration point with [`starlifter-os`](https://github.com/Overby-Industries/starlifter-os)'s
      fleet coordinator once that exists
- [ ] Refining sub-FSM design (see
      [`docs/refining-automation-draft.md`](docs/refining-automation-draft.md))
      — per-ore-type process control, still on microcontroller scale

## Phase 3 — Industrial Resource Processing

**Objective:** Refining and early advanced-manufacturing processes move
to certified hardware once real processing equipment exists.

- [ ] PLCnext and/or TwinCAT 3 introduced for refining (smelting, volatile
      extraction) — see [`docs/hardware-decision.md`](docs/hardware-decision.md)
      for why and when
- [ ] Safety-rated I/O for high-temperature/high-power processes
- [ ] First advanced-manufacturing process chosen and scoped (see
      [`docs/advanced-manufacturing-draft.md`](docs/advanced-manufacturing-draft.md))

**Corresponds to:** org roadmap Phase V (Industrial Pilot, 2030–2040) —
this is where "first profitable asteroid returns" needs real refining
automation behind it, not just extraction.

## Phase 4 — Space Agriculture

**Objective:** First automated grow-module control loop.

- [ ] Environmental control (lighting, temperature, humidity) automated
- [ ] Nutrient/water dosing automated
- [ ] Coordination boundary established with
      [`outpost-core`](https://github.com/Overby-Industries/outpost-core)
      for shared atmosphere systems
- See [`docs/space-agriculture-draft.md`](docs/space-agriculture-draft.md)
  for scope and open questions.

## Phase 5 — Full ISRU Civilization Stack

**Objective:** Extraction, refining, manufacturing, and agriculture
operating as one coordinated system feeding outposts and fleet logistics,
not four disconnected efforts.

- [ ] Cross-domain fault coordination (a refining fault and an agriculture
      fault may share root causes — e.g. power or atmosphere — that
      isolated per-domain FSMs won't catch alone)
- [ ] "A lot of PLCs" running consistently: shared conventions across
      every PLCnext/TwinCAT deployment in this repo, not ad hoc per
      process

**Corresponds to:** org roadmap Phase VI–VIII (Expansion through Solar
Civilization) — this is the automation backbone a closed-loop space
economy actually runs on.

---

## Status Summary

Phase 1 is the only phase with real code behind it. Everything from Phase
2 onward is design/sequencing, not a promise of dates — consistent with
the org roadmap's "TBD, requires funding" framing for everything past
Phase I.
