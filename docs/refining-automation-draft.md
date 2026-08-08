# Refining Automation — Draft

Not implemented yet — the `Refining` state in [`MinerPodFSM`](../firmware/lib/miner_pod_fsm)
currently just waits for a single `refineComplete` signal. This doc is
where the real process control for what happens *during* that state gets
designed, before it's built.

## Scope

Converting raw extracted material into usable feedstock, per the org
README's ISRU output streams:

- **Metals** (nickel, iron, PGMs) — ingot smelting from M-class asteroid ore.
- **Aggregates** — silicates into UHPC/basalt-fiber composite feedstock.
- **Volatiles** — H2/O2/CH4 extraction from ice/CO2.
- **Polymers** — Titan hydrocarbons into plastic feedstock (longer-term,
  location-dependent).

Each of these is a genuinely different process (thermal smelting vs.
mechanical/chemical aggregate production vs. cryogenic volatile capture),
which is why `MinerPodFSM`'s `Refining` state is deliberately a single
placeholder right now rather than a guess at a shared sub-FSM — see the
open question in [`pod-fsm-draft.md`](pod-fsm-draft.md) about whether
`Refining` needs its own sub-FSM per ore type, the same way
`power-and-propulsion`'s `MHD_Activation` has `MHDSubState`.

## Hardware path

This is very likely the **first real PLCnext/TwinCAT 3 use case** in this
repo (see [`hardware-decision.md`](hardware-decision.md)) — smelting and
volatile extraction involve higher power and higher temperatures than a
mobile mining pod, plus safety-rated I/O for things like furnace
temperature interlocks that a hobbyist microcontroller isn't rated for.

## Reporting

Refining events report via [`telemetry-protocol`](https://github.com/Overby-Industries/telemetry-protocol)'s
OTF-1 format — see that repo's `mining-refinery-report.json` example
(`action: "processed"`, `outcome.repurposedAs`).

## Not decided yet

- Per-ore-type sub-FSM design (smelting vs. aggregate vs. volatile
  extraction likely need genuinely different state sequences, not
  parameterized variants of one FSM).
- Temperature/process setpoints — entirely dependent on hardware not yet
  chosen; no numbers here are real until benchtop refining hardware exists
  to validate against, the same caution `power-and-propulsion/sim` applies
  to its physics models.
