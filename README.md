# Miner Automation

**Overby Industries — PLC and automation systems for drills, crushers, containment bags, and refinery pods.**

## Status: early design

Nothing is built yet. This repo exists to hold the design work as it
happens.

## Relationship to Other Repos

- **[`space-reclamation`](https://github.com/Overby-Industries/space-reclamation)**'s
  README calls for "PLC ladder logic or maybe JAC programming logic to
  automate most of the functions of the space miners / reclamation units"
  -- this repo is where that actually gets built, shared between mining
  pods and reclamation pods rather than duplicated in each.
- **[`telemetry-protocol`](https://github.com/Overby-Industries/telemetry-protocol)**
  (OTF-1) is the format a pod would use to report processing events
  (`action: "processed"`, `"refined"`) -- see the
  `mining-refinery-report.json` example in that repo.
- **[`power-and-propulsion`](https://github.com/Overby-Industries/power-and-propulsion)**'s
  `ShuttleFSM` is the closest existing precedent in this org for how a
  pod's control logic should be structured (non-blocking, watchdog-guarded,
  explicit fault/abort state) -- a miner pod's FSM should follow the same
  pattern, not reinvent it.

## Draft Scope

A miner/refinery pod's automation likely needs at least these phases (see
[`docs/pod-fsm-draft.md`](docs/pod-fsm-draft.md) for the state diagram):

1. **Anchoring** -- securing to the target body before any drilling starts.
2. **Containment deployment** -- expandable bag system, must be confirmed
   sealed before drilling (this is the "dustless" part of the mission --
   see the org README's containment-bag concept).
3. **Drilling / excavation**
4. **Crushing / processing**
5. **Refining** -- ingot smelting, volatile extraction, or aggregate
   production depending on ore type.
6. **Transfer** -- handoff to a Starlifter or storage.
7. **Fault / safe mode** -- same philosophy as `ShuttleFSM::AbortSafeMode`:
   a fault preempts every other state and recovery is deliberate, not
   automatic.

## Open Questions

- PLC ladder logic vs. a software FSM (like `ShuttleFSM`) running on an
  embedded controller -- which fits the actual hardware being used? This
  probably depends on decisions not yet made about the physical drill/
  crusher hardware.
- How much of the containment-bag confirmation logic needs redundant
  sensing (multiple seal sensors, voting logic) given a failure means
  uncontained debris -- directly against the zero-dust mission?

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) and [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md).
PLC programmers, automation engineers, and controls folks are especially
useful here.

## License

[Apache 2.0](LICENSE).
