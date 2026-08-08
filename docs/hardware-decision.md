# Hardware Decision: Controller Platform

Resolves the "PLC ladder logic vs. software FSM" open question this repo
started with.

## Decision

**Now:** plain C++ on microcontrollers already on hand — an Arduino Uno
and an ESP32. No ladder logic anywhere in this repo.

**Later, once industrial resource processors exist** (regolith/ore
refining hardware, advanced-manufacturing equipment): add **PLCnext
Technology** (Phoenix Contact) and/or **Beckhoff TwinCAT 3** for the
processes that need certified industrial I/O — both let you write actual
C++ instead of ladder, unlike traditional Allen-Bradley/Siemens-style PLC
programming.

## Why

The person building this has years of embedded C++ background and found
ladder logic (learned on Allen-Bradley/RSLogix) frustrating to work in —
particularly the amount of ceremony around timers compared to just writing
the logic directly. Given that, and that the near-term hardware is a
microcontroller, not a certified industrial controller, there's no reason
to use ladder logic here at all:

- **`MinerPodFSM`** (this repo, [`firmware/lib/miner_pod_fsm`](../firmware/lib/miner_pod_fsm))
  is plain, portable C++ — the exact same pattern as
  [`power-and-propulsion`](https://github.com/Overby-Industries/power-and-propulsion)'s
  `ShuttleFSM`: non-blocking, watchdog-guarded, explicit fault state with
  no automatic recovery.
- It builds for **both** boards already owned (`pio run -e uno`,
  `pio run -e esp32`) from the same source, via the board-specific
  watchdog macros in [`firmware/src/miner_pod.ino`](../firmware/src/miner_pod.ino)
  — AVR and ESP32 have different watchdog APIs, so that's the one place
  that's conditionally compiled; the FSM logic itself is fully portable.

## When PLCnext / TwinCAT actually comes in

Regolith/ore refining and advanced manufacturing (crystal growth, other
high-temperature or high-precision processes — see
[`docs/advanced-manufacturing-draft.md`](advanced-manufacturing-draft.md))
are a different risk class than a benchtop mining-pod prototype: higher
power, higher temperatures, and eventually needing UL/CE-style industrial
certification that a hobbyist microcontroller can't get you. That's the
point at which this repo should add:

- **PLCnext Technology** — native C++ (and C#) alongside IEC 61131-3, on
  real industrial-rated controllers running a real-time Linux base.
- **Beckhoff TwinCAT 3** — C++ development integrated into Visual Studio
  (TwinCAT C++/TcCOM modules), deployed to Beckhoff's industrial PCs;
  strong fit for motion-control-heavy processes (drives, crushers).

Both keep the "write real code, not ladder" property this decision is
built around — they're not a fallback to ladder logic, just a move to
certified hardware once the process demands it.

## What this doesn't decide

- *Which* process moves to PLCnext vs. TwinCAT vs. staying on a
  microcontroller — that depends on the actual hardware once it's
  acquired, not a rule set in advance.
- Whether the mining-pod FSM itself ever needs to move off a
  microcontroller — extraction/containment is exactly the kind of
  low-power, mobile-pod process that plain C++ on an ESP32 seems suited
  to indefinitely, but that's worth revisiting once real hardware exists.
