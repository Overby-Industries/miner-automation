# Advanced Manufacturing — Draft

Not started. This is the domain beyond basic refining: turning ISRU
feedstock into precision materials and components, in an environment
(microgravity, vacuum access, no convection) that's actually an
*advantage* for some processes, not just a constraint to work around.

## Why this belongs in this repo, not a new one

Advanced manufacturing is automation-heavy in exactly the same way mining
and refining are — process control, sensor feedback, fault handling — so
it follows the same `MinerPodFSM`-style pattern and the same
[hardware decision](hardware-decision.md) (microcontroller now, PLCnext/
TwinCAT for certified industrial processes later) rather than becoming its
own repo with its own conventions.

## Candidate process: crystal growth

Growing high-purity crystals (quartz/silicon and others) is a genuinely
well-precedented reason to do manufacturing in space, not just an
aspirational claim:

- Microgravity removes convection and sedimentation, which are two of the
  main sources of defects in ground-grown crystals. NASA and the ISS
  National Lab have run real crystal-growth experiments in orbit for
  exactly this reason (protein crystal growth is the most flown example;
  semiconductor-relevant crystal growth in microgravity has also been
  studied).
- For Overby specifically: ISRU silicates are already a planned output
  stream (see the org README and `power-and-propulsion`'s ISRU section) —
  high-purity quartz/silicon crystal growth would be a natural
  higher-value use of that feedstock instead of only structural
  aggregate/concrete.

This is a real, citable direction — but no specific furnace design, growth
method (Czochralski vs. vapor transport vs. something microgravity-native),
or process parameters are decided. That's real R&D, not something to guess
at in a scaffold doc.

## What automation this needs (once a process is chosen)

Whatever the specific method, the automation shape is likely similar to
refining: a controlled temperature/pressure ramp profile, sensor feedback
(temperature, growth rate if measurable), and fault handling for thermal
runaway or process deviation — same `MinerPodFSM`/fault-preemption
philosophy as everywhere else in this org, once there's a real process to
control.

## Not decided yet

- Which process(es) to pursue first — this needs real materials-science
  input, not a default assumption.
- Ground-based (Earth lab) prototyping vs. designing for microgravity from
  the start.
