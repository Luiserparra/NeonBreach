# NeonBreach — Milestone tracker

Source of truth: [`Plan_NeonBreach_UE5_EN.md`](Plan_NeonBreach_UE5_EN.md).  
Each `M<N>/M<N>.md` is a small, verifiable chunk (about 2–5 days) that ends in a playable or testable state.

**Status values:** `Not started` · `In progress` · `Done`  
A milestone is **Done** only after its acceptance criteria pass **and** `Learn/M<N>_Lesson.md` exists (see `.cursor/rules/milestone-learn.mdc`). `Learn/` is gitignored; `DevSpecs/` is tracked.

Tags follow the plan’s weekly versions at week-end milestones; mid-week chunks use a patch (e.g. `v0.1.1`).

| ID | Goal | Plan week | Days | Tag | Status |
|---|---|---|---|---|---|
| [M1](M1/M1.md) | Compiling NB framework + Enhanced Input; 2-client PIE movement | W1 D1–2 | 2 | `v0.0` | Done |
| [M2](M2/M2.md) | Server-authoritative hitscan, death, respawn; 3 clients in PIE | W1 D3–5 | 3 | `v0.1` | Not started |
| [M3](M3/M3.md) | ASC on PlayerState + AttributeSet; fire migrated to predicted `GA_Fire` | W2 D1–2 | 2 | `v0.1.1` | Not started |
| [M4](M4/M4.md) | Dash, Shock Orb, Barrier over the network; lag-tested at 150 ms | W2 D3–5 | 3 | `v0.2` | Not started |
| [M5](M5/M5.md) | HUD: health, energy, cooldowns, crosshair, hit marker | W3 D1–2 | 2 | `v0.2.1` | Not started |
| [M6](M6/M6.md) | Full match loop (warmup → victory → rematch) + scoreboard/kill feed | W3 D3–5 | 3 | `v0.3` | Not started |
| [M7](M7/M7.md) | Listen-server lobby: Host / Join IP / Ready / Start + travel to arena | W4 D1–2 | 2 | `v0.3.1` | Not started |
| [M8](M8/M8.md) | One Behavior Tree bot on the GAS pipeline; full lobby→match loop in a build | W4 D3–5 | 3 | `v0.4` | Not started |
| [M9](M9/M9.md) | Unreal Insights before/after + feel polish; documented perf table | W5 D1–5 | 5 | `v0.5` | Not started |
| [M10](M10/M10.md) | Edge cases, Shipping Windows build, video, itch.io, tag `v1.0` | W6 D1–5 | 5 | `v1.0` | Not started |

## Order (do not skip)

Foundation + networked hitscan (**M1–M2**) → GAS (**M3–M4**) → UI + match loop (**M5–M6**) → lobby + AI (**M7–M8**) → Insights (**M9**) → polish / build / video / itch.io (**M10**).

## Who does what (default)

- **You (editor, content, playtests, delivery):** Blueprints, maps, UMG layout, Input/GE/Niagara assets, Fab packs, PIE / packaged verification, video, itch.io.
- **Agent (C++ / specs):** gameplay classes, replication, GAS C++ abilities, BT tasks, C++ widget bases. Agent does **not** do your editor/content tasks unless you ask.
