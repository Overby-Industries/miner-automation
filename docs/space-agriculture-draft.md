# Space Agriculture Automation — Draft

Not started. Food production for a self-sufficient outpost/colony is
control-systems-heavy in the same way mining and refining are — this is
the automation for growing it, not the biology/agronomy itself.

## Why this belongs in this repo

A self-sufficient space-based civilization needs closed-loop food
production, not just materials extraction — this is the ISRU mission's
other half. The automation problem (sensor feedback, actuator control,
fault handling for a process that can't just "pause" the way a mining pod
can) fits the same pattern as everything else here, and the same
[hardware decision](hardware-decision.md) applies: microcontroller-scale
control for a single grow module now, PLCnext/TwinCAT for a full outpost
farm ("a lot of PLCs," per the mission) once that scale exists.

## Real precedent

Controlled-environment agriculture in space isn't hypothetical — NASA's
Veggie and Advanced Plant Habitat (APH) experiments on the ISS have grown
food crops under automated lighting/watering/monitoring for years. This
repo's job is the automation layer for something in that category, scaled
toward outpost-level production instead of an experiment-sized chamber.

## What a grow-module control loop likely needs

- **Environmental control**: lighting cycle (spectrum + duration),
  temperature, humidity.
- **Nutrient/water dosing**: hydroponic or aeroponic delivery, timed and
  quantity-controlled.
- **Atmosphere balance**: CO2/O2 monitoring — relevant both to plant health
  and, at outpost scale, to [`outpost-core`](https://github.com/Overby-Industries/outpost-core)'s
  life-support boundary (that repo explicitly scopes life support out for
  now; a grow module's atmosphere interaction is one of the first places
  that boundary will need real coordination between the two repos).
- **Fault handling**: same philosophy as `MinerPodFSM` — a fault (thermal,
  atmosphere, dosing failure) should preempt normal cycling and alert
  rather than silently continue watering/lighting on bad sensor data.

## Not decided yet

- Which crops/growing method first — real agronomy input needed, not
  assumed here.
- Where the line is between this repo's automation and `outpost-core`'s
  life-support systems once both exist — they'll need to coordinate
  (shared atmosphere) without either repo owning the other's scope.
