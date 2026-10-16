# NeonBreach — Multiplayer Arena in UE5 (C++ · GAS · 2–4 players)

> Project plan for **Luis Ernesto Parra-Beltran**
> Goal: portfolio demo for Unreal gameplay/multiplayer openings (e.g., Triplo Z, UnitedCode).
> Duration: **5 weeks of building + 1 week of buffer/polish/delivery**.
> Golden rule: **small, finished, and polished > big and half-done.**

---

## 1. One-sentence pitch

**NeonBreach** is a 2–4 player (plus one bot) third-person arena shooter in which each player manages an **Energy** bar to fire off dash, electric orb, and shield abilities: first to 10 kills wins, and the match restarts within seconds.

---

## 2. Scope (in / out)

| ✅ IN (MVP) | ❌ OUT (out of scope) |
|---|---|
| 1 playable character (template mannequin) with 1 hitscan weapon | Multiple characters/classes or hero selection |
| 3 GAS abilities (Dash, Shock Orb, Barrier) with cost, cooldown, and effects | Talent tree, progression, inventory, loot |
| 1 small arena map (blockout with primitives + materials) | Multiple maps, map rotation |
| Free-For-All mode, 2–4 human players + 1 bot | Teams, alternate modes (CTF, KOTH) |
| Listen server, LAN / direct IP connection | Dedicated server, matchmaking, cloud servers |
| Simple lobby: Host / Join by IP / Ready / Start | Friends system, chat, accounts, persistence |
| UMG HUD: health, energy, cooldowns, scoreboard, timer, kill feed | Full options menu (only sensitivity + volume if time allows) |
| Loop: lobby → warmup → match → victory → restart | Replays, killcam, spectator mode |
| 1 bot with a Behavior Tree (patrols, chases, shoots) | Complex AI, multiple enemy types, adaptive difficulty |
| Prediction for firing (cosmetic FX) and Dash (GAS) | Lag compensation with hitbox rewind (documented as a "next step") |
| Unreal Insights pass with before/after | Exhaustive GPU/console optimization |
| Windows build on itch.io + 1–2 min video + README | Mac/Linux/mobile builds, anti-cheat |

---

## 3. Combat decision: **hitscan shooting** (+ 1 projectile via an ability)

**Recommendation: hitscan as the primary weapon.** Melee is out.

| Criterion | Melee | Hitscan | Projectile |
|---|---|---|---|
| Networked hit detection | Hard: depends on animation timing, replicated montages, hit windows, root motion | Simple: one `LineTrace` on the server | Moderate: replicated actor + collision |
| Animation content required | High (combos, reactions, montages) | Low (aim pose + recoil) | Low |
| Feel under lag | Very noticeable ("ghost" hits) | Acceptable with predicted FX | Acceptable |
| Risk over 4–6 weeks | **High** | **Low** | Medium |
| What it shows a recruiter | Animation/combat design | Server authority, validation, RPCs, cosmetic prediction | Actor replication |

**Why:**
- Hitscan lets you demonstrate the classic **client predicts cosmetics → server validates and applies damage** pattern in just a few lines, and it is easy to explain in the README and in interviews.
- The free assets in the third-person template don't include decent melee animations; for hitscan, a single pose and a muzzle flash are enough.
- The **Shock Orb** (ability 2) adds a **replicated projectile**, so the portfolio covers both patterns without the cost of melee.

---

## 4. Tech stack

### 4.1 Engine and project
- **Unreal Engine 5.8 or newer** (lock it on day 1 — e.g. 5.8 — and don't change it during the project).
- Base project: **Third Person (C++)**, without Starter Content (add it only if you need placeholder sounds).
- IDE: Visual Studio 2022 or Rider. Version control: **Git + Git LFS** (`*.uasset`, `*.umap`).

### 4.2 Modules and plugins
| Item | Purpose |
|---|---|
| `GameplayAbilities`, `GameplayTags`, `GameplayTasks` (modules in `Build.cs`) + **Gameplay Abilities** plugin | GAS |
| `EnhancedInput` | Input (enabled by default in UE5 templates) |
| `UMG`, `Slate`, `SlateCore` | HUD and menus |
| `AIModule`, `NavigationSystem` | Behavior Tree bot |
| `Niagara` | VFX for firing, orb, shield |
| `OnlineSubsystem` + `OnlineSubsystemNull` | LAN / direct IP sessions (no Steam in the MVP) |
| `NetCore` | Replication utilities (optional push model) |

### 4.3 C++ vs Blueprint (project rule)
| C++ (logic) | Blueprint (data/content) |
|---|---|
| Character, movement, weapon, tracing, damage | `BP_` children of the C++ classes to assign meshes, sounds, FX |
| GameMode, GameState, PlayerState, PlayerController | Tuning values (damage, fire rate, cooldowns) in Data Assets / GEs |
| AbilitySystemComponent, AttributeSet, base GameplayAbilities | Gameplay Effects (`GE_`) configured in the editor |
| Match logic (phases, score, respawn, victory) | Visual widget layout (UMG Designer) |
| Behavior Tree tasks/services and AIController | Behavior Tree and Blackboard (assets), EQS if used |
| Enhanced Input bindings | Input Actions / Input Mapping Context (assets) |

**Rule:** if it has an `if`, networking, or state → C++. If it's a number, a mesh, or a color → Blueprint/Data Asset. Widgets are created in C++ as a base `UUserWidget` with `BindWidget`, and the BP only handles layout.

### 4.4 Minimal GAS layout
```
ANBPlayerState
 └─ UAbilitySystemComponent (ReplicationMode = Mixed)
 └─ UNBAttributeSet
      Health, MaxHealth, Energy, MaxEnergy, EnergyRegen
      IncomingDamage (meta attribute, not replicated)

ANBCharacter (implements IAbilitySystemInterface → returns the PlayerState's ASC)
ANBBotCharacter (own ASC on the Character, ReplicationMode = Minimal)

UNBGameplayAbility (C++ base)
 ├─ GA_Fire        (input: Fire)      — hitscan, no cooldown, fire rate via tag
 ├─ GA_Dash        (input: Ability1)
 ├─ GA_ShockOrb    (input: Ability2)
 └─ GA_Barrier     (input: Ability3)

Gameplay Effects (assets)
 GE_Damage (Instant, SetByCaller Data.Damage → IncomingDamage)
 GE_Cost_Dash / GE_Cost_Orb / GE_Cost_Barrier (Instant, -Energy)
 GE_Cooldown_Dash / _Orb / _Barrier (Duration, grants Cooldown.Ability.X)
 GE_EnergyRegen (Infinite, periodic)
 GE_Barrier (Duration 3 s, tag State.Barrier → reduces damage by 60% in PostGameplayEffectExecute)
 GE_Slow (Duration 2 s, -40% speed)
 GE_InitStats (Instant on spawn)

Gameplay Tags
 Ability.Fire / Ability.Dash / Ability.ShockOrb / Ability.Barrier
 Cooldown.Ability.Dash / .ShockOrb / .Barrier
 State.Dead / State.Barrier / State.Slowed
 Data.Damage
 Event.Hit
```
- ASC on the **PlayerState** so that health/state survive pawn respawns, and to follow the pattern recommended in the GAS community documentation.
- Initialize the ASC in `PossessedBy` (server) and `OnRep_PlayerState` (client).
- In recent UE5 versions, `UAbilitySystemGlobals::InitGlobalData()` is called automatically; if your version requires it, call it in the `UAssetManager` or `GameInstance`.

---

## 5. Multiplayer architecture

### 5.1 Listen server vs dedicated — **Listen server**
- A **dedicated server build requires compiling the engine from source**; that eats up days you don't have.
- For an itch.io demo, the player who clicks "Host" is the server; everyone else clicks "Join" by IP (LAN or with a forwarded port).
- In the editor, **do** test with `Net Mode = Play As Client` (PIE spins up an internal dedicated server): this forces nothing to depend on "the host is also a player."
- Document in the README that the architecture is *server-authoritative* and that moving to a dedicated server is a matter of configuration + build, not a rewrite.

### 5.2 What gets replicated
| Data | Where it lives | How |
|---|---|---|
| Health, Energy | AttributeSet (PlayerState) | GAS replication (`DOREPLIFETIME_CONDITION_NOTIFY`, `REPNOTIFY_Always`) |
| Cooldowns and states | ASC | Replicated tags and GEs (Mixed: full to the owner, minimal to everyone else) |
| Kills, Deaths | `ANBPlayerState` | `UPROPERTY(ReplicatedUsing=OnRep_Score)` |
| Match phase, time remaining, winner | `ANBGameState` | Replicated + `OnRep_MatchPhase` |
| Position/movement | `UCharacterMovementComponent` | Built-in replication and prediction |
| Shock Orb projectile | `ANBProjectile` | `bReplicates = true`, replicated `ProjectileMovementComponent` |
| Firing FX for other players | Character | `NetMulticast Unreliable` (or GameplayCue) |
| Kill feed | GameState | `NetMulticast Reliable` with (Killer, Victim) |
| Lobby Ready state | PlayerState | Replicated bool |

### 5.3 What gets predicted (client)
1. **Firing (GA_Fire, LocalPredicted):** the client plays the muzzle flash, sound, tracer, and local impact *immediately*; the server runs its own trace and applies `GE_Damage`. If the server doesn't confirm, all that's lost is a cosmetic effect.
2. **Dash (GA_Dash, LocalPredicted):** uses `UAbilityTask_ApplyRootMotionConstantForce`, which integrates with `CharacterMovementComponent` prediction → no rubber-banding at 150 ms of lag. **This is the portfolio's "star" prediction mechanic.**
3. **Cost and cooldown:** GAS predicts the cost/cooldown GE on the client; the cooldown UI starts instantly.

### 5.4 Authority map
| Action | Client | Server |
|---|---|---|
| Input and movement | Sends input, predicts | Validates and corrects (CMC) |
| Ability activation | Requests + predicts | Authorizes (cost, cooldown, tags), rolls back on failure |
| Damage / death | ❌ never | ✅ trace, GE_Damage, State.Dead |
| Shot validation | Sends origin/direction | Re-traces from its own weapon position; rejects absurd distances/angles |
| Projectile spawning | ❌ | ✅ |
| Score, phases, victory, respawn | ❌ (display only) | ✅ GameMode |
| AI | ❌ | ✅ (the AIController exists only on the server) |
| UI | ✅ (reads replicated state) | — |

### 5.5 Lag testing
- **Editor Preferences → Level Editor → Play → Multiplayer Options → Enable Network Emulation**, *Average* and *Bad* profiles, applied to both client and server.
- Console: `Net PktLag=150`, `Net PktLagVariance=30`, `Net PktLoss=3`.
- `stat net`, `p.NetShowCorrections 1` to see CMC corrections.
- Acceptance criterion: at **150 ms + 3% packet loss**, firing and dashing feel instant for the local player, and there are no visible corrections when dashing.

---

## 6. Loop design

```
[MainMenu/Lobby map]
  Host (creates LAN / listen session) ── Join (IP or LAN list)
        │
  Lobby: player list + Ready  → Host presses Start (≥2 ready)
        │  ServerTravel (seamless, with a transition map)
        ▼
[Arena map]
  WaitingToStart → Warmup (5 s, countdown, no damage)
        ▼
  InProgress: FFA, 3 s respawn at the spawn farthest from enemies
  Win: first to 10 kills  or  highest score when 5:00 runs out
        ▼
  PostMatch (10 s): victory screen + final scoreboard, input locked
        ▼
  Host: "Rematch" (RestartGame / ServerTravel to the same map)
        or "Back to lobby" (ServerTravel to Lobby)
```

- **Score:** +1 per kill; the bot counts as a player on the scoreboard (it can be killed and can kill you). Suicide/fall = -1.
- Phases live in a replicated `enum class ENBMatchPhase` on `ANBGameState`; the HUD reacts in `OnRep`.
- **Seamless travel doesn't work well in PIE:** test it in *Standalone* / a packaged build starting in week 4.
- "Make it feel like a game": countdown with sound, kill feed, hit marker, victory sound, winner camera in PostMatch.

---

## 7. The 3 abilities

| | **Dash** | **Shock Orb** | **Barrier** |
|---|---|---|---|
| Input (Enhanced Input) | `IA_Ability1` — Shift / LB | `IA_Ability2` — Q / RB | `IA_Ability3` — E / Y |
| Cost | 20 Energy | 35 Energy | 40 Energy |
| Cooldown | 4 s | 8 s | 15 s |
| Effect | 900 u burst in the movement direction (0.2 s) | Slow projectile (1500 u/s); on impact, 35 damage in a 300 u radius + `GE_Slow` for 2 s | 3 s of 60% damage reduction + visual overlay |
| Network policy | LocalPredicted | LocalPredicted (anim/sound); **the projectile is spawned only by the server** | LocalPredicted (tag) → the server confirms the GE |
| How it looks over the network | The owner moves instantly (predicted root motion); others see it via normal CMC replication + a trail GameplayCue | The owner sees the animation instantly; everyone sees the replicated orb; explosion via GameplayCue | GameplayCue `GC.Barrier` (looping) visible to everyone while the tag is active |
| What it demonstrates | Movement prediction with GAS | Server-spawned replicated actor, area damage, duration GE | Tags, damage modification in `PostGameplayEffectExecute`, persistent cues |

**Base weapon (GA_Fire):** hitscan, 12 damage per bullet, 8 shots/s, ×1.5 headshot (optional), no energy cost. Energy: 100 max, regenerates 10/s.
**Health:** 100. Time-to-kill ~1 s with every shot landing: fast enough for a dynamic video.

---

## 8. AI: **Behavior Tree** (chosen) vs StateTree

**I'm choosing Behavior Tree** because:
- It's the most thoroughly documented system and the one most recruiters/leads recognize in an interview.
- Mature debugging tools (live tree visualization, the **Gameplay Debugger** on the `'` key) — key for capturing evidence in the video.
- Direct integration with **Blackboard, AIPerception, and EQS**.
- StateTree is newer and powerful for state-based logic, but its learning curve and changes between versions add risk. Mention it in the README as a "next step / evaluated alternative."

**C++ components:** `ANBAIController` (AIPerception with Sight), `UBTService_UpdateTarget`, `UBTTask_FireAtTarget` (activates `GA_Fire` by tag via the ASC — the bot uses the **same** GAS pipeline as the players), `UBTTask_UseAbility`.

**Blackboard:** `TargetActor` (Object), `LastKnownLocation` (Vector), `PatrolLocation` (Vector), `HasLineOfSight` (Bool).

```
Root
└─ Selector
   ├─ [Decorator: TargetActor is set & HasLineOfSight]
   │   Sequence "Combat"
   │   ├─ Service: UpdateTarget (0.3 s)
   │   ├─ Task: RotateToFaceBBEntry (TargetActor)
   │   ├─ Simple Parallel
   │   │   ├─ Task: FireAtTarget (0.8 s burst)
   │   │   └─ Task: MoveTo (strafe / EQS point at 600–1000 u)  [optional]
   │   └─ Task: UseAbility(Dash) if Health < 40 [decorator]
   ├─ [Decorator: LastKnownLocation is set]
   │   Sequence "Investigate"
   │   ├─ Task: MoveTo (LastKnownLocation)
   │   ├─ Task: Wait 1.5 s
   │   └─ Task: ClearBBValue(LastKnownLocation)
   └─ Sequence "Patrol"
       ├─ Task: FindRandomReachablePoint → PatrolLocation
       ├─ Task: MoveTo (PatrolLocation)
       └─ Task: Wait 2 s
```
- The bot lives only on the server; its character replicates like any other.
- Artificial aim spread (±3°) so it isn't an aimbot.

---

## 9. Assets and level

**Decision: 2 Fab Free (Personal) packs + template mannequin + own VFX/UI.** Rationale: Underground Sci-Fi covers the modular arena; Low Poly Weapons Lite covers weapon meshes; the character stays the Third Person template mannequin so the project isn't built on a sample. Everything is free under Fab's Personal license (team &lt;$100k revenue/funding).

| Need | Source (free, safe) |
|---|---|
| **Level / arena** | **[Underground Sci-Fi](https://www.fab.com/listings/b2f733e9-9150-4f7e-b4c3-247a2b64a219)** (GeorgeShachnev) — Fab Free, Personal license. UE listed through **5.8**. Modular sci-fi bunker/station environment for the arena. |
| **Weapons (meshes)** | **[Low Poly Weapons Lite](https://www.fab.com/listings/ecbb5891-da50-4488-ba94-a13aa742168a)** (JustCreate) — Fab Free, Personal license. Official listing UE **5.2–5.7** (may open in 5.8; **verify on day 1**). |
| **Character and locomotion** | **Manny/Quinn** mannequins from the UE5 Third Person template (no external character pack). |
| **VFX** | Own Niagara from editor templates (Fountain, Burst, Beam, etc.): Dash trail, Shock Orb, Barrier, muzzle/impact. |
| **UI** | Own UMG (C++ widget base + BP layout); do not use Lyra's HUD or other sample UIs. |
| **Sound** | Placeholders / engine **Starter Content**; expose `USoundBase*` in Data Assets so they can be swapped without code changes. |

**Kit rules**
- Do **not** base the project on **Lyra Starter Game**. Create your own Third Person C++ project (your GAS, networking, and loop). Lyra is optional study material later only — not the project base and not an MVP asset source.
- Day 1: lock **UE 5.8+**, download both packs from Fab, confirm Low Poly Weapons Lite opens on your build (if not, use a Modeling Mode placeholder weapon until resolved).
- Lighting: Lumen with 1 Directional Light + SkyAtmosphere + emissive lights from the pack; post process with a bit of bloom.
- **Required:** `CREDITS.md` in the repo listing both Fab packs (name, author, URL), **Personal** license and the &lt;$100k threshold note, plus Epic's mannequin/template and Starter Content if used.

---

## 10. Schedule (5 weeks + 1 buffer)

> Every week ends with: **a packaged playable build + a tagged commit** (`v0.1`, `v0.2`, …). If it isn't verifiable, it isn't done.

### Week 1 — Foundation, character, networked shooting
| Day | Deliverable |
|---|---|
| 1 | Third Person C++ project, Git + LFS, `.gitignore`, folder structure, empty base classes (see §14) |
| 2 | `ANBCharacter` with Enhanced Input (Move, Look, Jump, Fire, Ability1–3) in C++ |
| 3 | Hitscan weapon: server-side trace via `Server RPC`, temporary damage (no GAS yet), multicast FX |
| 4 | Replicated health + death + basic respawn in `ANBGameMode` |
| 5 | Test with 3 clients in PIE + "Average" Network Emulation; map blockout v0 |

**Verifiable milestone:** 3 players in PIE shoot each other, die, and respawn; the host has no logic advantage.

### Week 2 — GAS
| Day | Deliverable |
|---|---|
| 1 | ASC on the PlayerState, AttributeSet (Health, Energy…), client/server init |
| 2 | Migrate firing to `GA_Fire` (LocalPredicted) + `GE_Damage` with SetByCaller |
| 3 | `GA_Dash` with predicted root motion + cost/cooldown |
| 4 | `GA_ShockOrb` (server-spawned projectile) + `GE_Slow` |
| 5 | `GA_Barrier` + GameplayCues; test at 150 ms / 3% loss, document in `docs/networking.md` |

**Milestone:** 3 abilities working over the network with cost and cooldown; dash with no corrections under lag.

### Week 3 — UI and match loop
| Day | Deliverable |
|---|---|
| 1 | Base C++ HUD (`UNBHUDWidget` with `BindWidget`): health, energy |
| 2 | Cooldowns (listening to the ASC's `Cooldown.*` tags), hit marker, crosshair |
| 3 | `ANBGameState` with phases, timer, winner; scoreboard (Tab) and kill feed |
| 4 | Warmup → InProgress → PostMatch → Rematch |
| 5 | Victory screen; packaged build v0.3 tested on 2 PCs/2 instances over LAN |

**Milestone:** a complete match from start to finish without touching the console.

### Week 4 — Lobby, sessions, AI
| Day | Deliverable |
|---|---|
| 1 | MainMenu/Lobby map: Host / Join IP / Find LAN (`OnlineSubsystemNull`) |
| 2 | Lobby with player list + Ready + Start; seamless ServerTravel with a transition map |
| 3 | `ANBAIController` + AIPerception + Blackboard + Behavior Tree (Patrol/Investigate) |
| 4 | Bot combat using GAS (`GA_Fire`, defensive Dash); the bot appears on the scoreboard |
| 5 | Full lobby → match → victory → lobby loop in a packaged build |

**Milestone:** full loop with 2 humans + 1 bot in a Windows build.

### Week 5 — Performance, feel polish, documentation
| Day | Deliverable |
|---|---|
| 1 | **"Before" Insights:** capture with `-trace=default,net -NetTrace=1` (or `-trace=cpu,gpu,frame,net`), 4 players + bot, 2 min; screenshots in `docs/perf/before/` |
| 2–3 | Optimizations (see list below) |
| 4 | **"After" Insights** with the same scenario; comparison table in `docs/perf/README.md` |
| 5 | Feel: screen shake, sounds, light hit stop, final map lighting; README v1 |

**Candidate optimizations (pick 3–4 measurable ones):**
- Disable `Tick` on actors/components that don't need it (`PrimaryActorTick.bCanEverTick = false`).
- Tune `NetUpdateFrequency` / `MinNetUpdateFrequency` for the PlayerState, projectiles, and pickups.
- Appropriate `NetCullDistanceSquared` for FX/cosmetic actors; FX only via GameplayCue/unreliable multicast.
- Pooling or short *lifetimes* for projectiles/Niagara; disable `Generate Overlap Events` where unused.
- Reduce the frequency of the BT Service and AIPerception.
- Measure with `stat unit`, `stat game`, `stat net`, and per-actor bandwidth in Networking Insights.

**Milestone:** before/after table with at least 2 metrics (Game thread ms, KB/s per client, or bits per actor).

### Week 6 — Buffer, video, itch.io
| Day | Deliverable |
|---|---|
| 1–2 | Bugs and edge cases (client disconnect, host leaves, tie, 4 players) |
| 3 | **Shipping Windows** build, tested on a clean PC (prerequisites installer included) |
| 4 | 1–2 min video (script in §11) + GIFs for the README |
| 5 | itch.io page (description, controls, how to host, screenshots, zip), final README, tag `v1.0` |

---

## 11. Definition of "done" (portfolio checklist)

**Gameplay and networking**
- [ ] 2–4 players + 1 bot complete a match without errors in a packaged build
- [ ] All damage/score/phase logic runs only on the server (`HasAuthority()` verifiable in code)
- [ ] Predicted firing and Dash; tested at 150 ms + 3% loss with no visible corrections
- [ ] 3 GAS abilities with cost, cooldown, effect, and GameplayCue visible to everyone
- [ ] A disconnecting client doesn't break the match

**Loop and UI**
- [ ] Lobby → warmup → match → victory → rematch/lobby without the console
- [ ] HUD: health, energy, 3 cooldowns, timer, scoreboard, kill feed, hit marker
- [ ] Controls shown on the start or pause screen

**Technical quality**
- [ ] Before/after Insights documented with screenshots and a table
- [ ] No warnings from project code at compile time; no errors in the Output Log during a match
- [ ] Gameplay in C++, Blueprints for data/content only (verifiable in the README)

**Presentation**
- [ ] Public repo with a complete README, GIFs, and an architecture diagram
- [ ] Clean commit history (Conventional Commits: `feat:`, `fix:`, `perf:`, `docs:`, `refactor:`), weekly tags
- [ ] 1–2 min video: 0–10 s hook (intense match) · 10–40 s abilities · 40–70 s networking with side-by-side emulated lag · 70–90 s AI with Gameplay Debugger + before/after Insights · close with a link to the repo
- [ ] Windows build on itch.io with instructions for hosting/joining
- [ ] `CREDITS.md` listing every asset used and its source

---

## 12. README outline (repo)

1. **NeonBreach** — gameplay GIF + 1-line pitch + links (video, itch.io)
2. **Features** — short list with what each one demonstrates
3. **Controls**
4. **How to play / host** — Host, Join by IP, default port 7777 (UDP), LAN note
5. **Architecture** — class diagram (GameMode, GameState, PlayerState+ASC, Character, Controller, AIController)
6. **Network model** — listen server, authority map, what's replicated, what's predicted, lag testing
7. **GAS** — layout (ASC on PlayerState, attributes, abilities, effects, tags) and the flow of an ability
8. **AI** — why Behavior Tree, tree diagram, StateTree as an evaluated alternative
9. **C++ vs Blueprint** — the rule used and a table of what lives where
10. **Performance** — test scenario, Insights captures, before/after table, what was changed
11. **Decisions and trade-offs** — hitscan vs melee, listen vs dedicated, primitives vs assets
12. **Known limitations and next steps** — lag compensation with rewind, dedicated server, Steam, StateTree
13. **Building from source** — exact UE version, IDE, Git LFS, steps
14. **Repo structure** and **Credits/licenses**

---

## 13. Risks and cut order

| Risk | Early warning sign | Mitigation |
|---|---|---|
| GAS takes longer than expected | By the end of week 2, there aren't 2 abilities working over the network | Keep the ASC/AttributeSet and cut down to 2 abilities |
| Seamless travel / sessions cause problems | Week 4, day 2 without stable travel | Use `open <IP>` / non-seamless `ServerTravel`; minimal lobby |
| Bugs that only show up in builds | First build comes too late | Package **every Friday** starting in week 1 |
| Internet connection (NAT) | Testers can't join | Document LAN/port 7777; the video shows 4 local clients |
| Visual scope creep | Hours spent on materials/VFX before week 5 | Visual polish only in weeks 5–6 |

**Cut order (from first to be cut to last):**
1. Find LAN (keep only Join by IP)
2. Headshots, animated kill feed, screen shake
3. Bot EQS / strafing (Patrol + Chase + Fire remain)
4. Ability 3 (Barrier) → 2 abilities remain
5. Lobby with Ready → direct Start button for the host
6. Seamless travel → simple travel
7. **Never cut:** server authority, Dash prediction, full loop, basic HUD, before/after Insights, README, video, itch.io build

---

## 14. Concrete next action (day 1)

1. **Create the project:** Launcher → **UE 5.8 or newer** (lock it; don't change versions mid-project) → Games → *Third Person* → **C++**, no Starter Content (or add it only for placeholder sounds), name `NeonBreach`. Download Underground Sci-Fi + Low Poly Weapons Lite from Fab and verify they open on your build.
2. **Git:** `git init`, Unreal `.gitignore` (`Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/`), `git lfs track "*.uasset" "*.umap"`, first commit `chore: initial UE5 C++ project`. Create the repo on GitHub and push.
3. **Plugins:** enable *Gameplay Abilities* (Edit → Plugins), restart the editor.
4. **`NeonBreach.Build.cs`:** add `GameplayAbilities`, `GameplayTags`, `GameplayTasks`, `EnhancedInput`, `UMG`, `AIModule`, `NavigationSystem`, `Niagara`, `OnlineSubsystem`, `OnlineSubsystemUtils`.
5. **Empty C++ classes** (Tools → New C++ Class), `NB` prefix:
   `ANBCharacter`, `ANBPlayerController`, `ANBPlayerState`, `ANBGameMode`, `ANBGameState`, `UNBAttributeSet`, `UNBGameplayAbility`, `ANBAIController`, `ANBProjectile`, `UNBHUDWidget`.
6. **Content folders:** `Content/NeonBreach/{Core, Characters, Abilities/{GA,GE,Cues}, Input, UI, AI, Maps, Materials, VFX, Audio, Data}`.
7. **Input:** create `IMC_Default` and `IA_Move`, `IA_Look`, `IA_Jump`, `IA_Fire`, `IA_Ability1`, `IA_Ability2`, `IA_Ability3`, `IA_Scoreboard`.
8. **Maps:** `L_MainMenu`, `L_Lobby`, `L_Arena`, `L_Transition`; Project Settings → Maps & Modes: default GameMode `BP_NBGameMode`, startup map `L_MainMenu`.
9. **Day's verification:** compiles with no errors; PIE with 2 players in *Play As Client* move using Enhanced Input from C++. Commit `feat: base gameplay classes and input setup`, tag `v0.0`.
