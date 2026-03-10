# Telemetry Feature Plan (Teleplot + VaporView)

## Goal

Add structured serial telemetry to the traffic light firmware so runtime behavior can be visualized in Teleplot and VaporView, while preserving existing human-readable debug logs and keeping interrupt handling safe.

## Non-Goals

- No change to traffic logic timing or state behavior.
- No protocol dependency beyond plain serial text.
- No ISR-heavy telemetry work (ISR stays minimal).

## Proposed PR Scope

### In Scope

- Add telemetry emit helpers in `src/main.cpp`.
- Add compile-time feature flags to enable/disable telemetry output.
- Emit low-cardinality signals at key state/phase boundaries.
- Add README section for Teleplot/VaporView setup and signal dictionary.
- Add API docs section describing telemetry fields and expected values.

### Out of Scope (Follow-up)

- Binary protocol telemetry.
- Web dashboard integration.
- Persistent logging to storage.
- Multi-board sync telemetry.

## Branch and PR Strategy

- Branch name: `feature/telemetry-teleplot-vaporview`
- PR title: `feat: add serial telemetry for Teleplot and VaporView`
- Keep as a separate PR from prior docs/lint/license changes.

## Design Overview

## Implementation Overview

Use this order to implement the feature with low risk and easy review.

### Step 1: Create Branch and Baseline Build

- Create branch: `feature/telemetry-teleplot-vaporview`
- Run baseline build: `pio run`
- Confirm current behavior is unchanged before edits.

Commands:

```bash
git checkout -b feature/telemetry-teleplot-vaporview
pio run
```

Why:

- Starting from a clean feature branch keeps PR scope isolated.
- A baseline build confirms the project compiles before telemetry edits.

### Step 2: Add Telemetry Infrastructure in `src/main.cpp`

- Add compile-time flags:
  - `ENABLE_TELEMETRY`
  - `ENABLE_DEBUG_LOGS`
  - `TELEMETRY_HEARTBEAT_MS`
- Add helper functions:
  - `emitMetric(const char* name, long value)`
  - `emitPinState(const char* name, uint8_t pin)`
- Add state variables:
  - `volatile bool buttonEdgePending`
  - `unsigned long lastHeartbeatMs`

Code changes (add near top of `src/main.cpp`):

```cpp
#define ENABLE_TELEMETRY 1
#define ENABLE_DEBUG_LOGS 1
#define TELEMETRY_HEARTBEAT_MS 250

#if ENABLE_DEBUG_LOGS
  #define DBG_PRINT(x) Serial.print(x)
  #define DBG_PRINTLN(x) Serial.println(x)
#else
  #define DBG_PRINT(x) do {} while (0)
  #define DBG_PRINTLN(x) do {} while (0)
#endif

volatile bool buttonEdgePending = false;
unsigned long lastHeartbeatMs = 0;

void emitMetric(const char* name, long value) {
#if ENABLE_TELEMETRY
  Serial.print(name);
  Serial.print(':');
  Serial.println(value);
#else
  (void)name;
  (void)value;
#endif
}

void emitPinState(const char* name, uint8_t pin) {
  emitMetric(name, digitalRead(pin));
}
```

Why:

- Feature flags allow easy enable/disable without deleting code.
- Dedicated emit helpers keep telemetry formatting consistent (`name:value`).

### Step 3: Apply ISR-Safe Event Capture

- Keep ISR (`stopTraffic`) flag-only.
- Set `stopRequested = true` and `buttonEdgePending = true` in ISR.
- Do not call `Serial.print*` from ISR.

Code changes (replace `stopTraffic()`):

```cpp
void stopTraffic() {
  if (!stopSequenceRunning) {
    stopRequested = true;
    buttonEdgePending = true;
  }
}
```

Why:

- ISR must be fast and deterministic.
- Printing in ISR can cause latency and timing side effects.

### Step 4: Emit Telemetry at State Boundaries

- In `setup()`: emit baseline metrics once.
- In `loop()`: emit normal state metrics and throttled `wait_ms` heartbeat.
- Before transition delay: emit `state=1`.
- In `runStopSequence()`: emit `state`, `phase`, output toggles, and `seq_total_ms`.

Code changes (representative snippets):

```cpp
// setup(): baseline snapshot
emitMetric("state", 0);
emitMetric("stop_requested", 0);
emitMetric("stop_running", 0);
emitPinState("led_green", grn_led);
emitPinState("led_amber", amb_led);
emitPinState("led_red", red_led);
emitPinState("led_builtin", LED_BUILTIN);
emitPinState("buzzer_on", buzzer);
```

```cpp
// loop(): while waiting for button
while (!stopRequested) {
  unsigned long now = millis();
  if (buttonEdgePending) {
    emitMetric("btn_edge", 1);
    buttonEdgePending = false;
  }
  if (now - lastHeartbeatMs >= TELEMETRY_HEARTBEAT_MS) {
    emitMetric("wait_ms", now);
    lastHeartbeatMs = now;
  }
}

emitMetric("state", 1); // transition delay
delay(3000);
stopRequested = false;
emitMetric("stop_requested", 0);
```

```cpp
// runStopSequence(): phase transitions
stopSequenceRunning = true;
startTime = millis();
emitMetric("state", 2);
emitMetric("stop_running", 1);
emitMetric("phase", 10);

// ... phase 20/30/40/50 emitted at boundaries ...

stopTime = millis();
emitMetric("seq_total_ms", stopTime - startTime);
emitMetric("phase", 60);
emitMetric("stop_running", 0);
emitMetric("state", 0);
stopSequenceRunning = false;
```

Why:

- Transition-point emission gives useful plots without flooding serial output.
- Heartbeat throttling keeps timings stable.

### Step 5: Update Documentation

- `README.md`: add Teleplot/VaporView setup and signal dictionary.
- `docs/API.md`: add telemetry signal definitions, phase map, and ISR safety note.

Commands:

```bash
code README.md docs/API.md
```

Additions to include:

- Signal table (`state`, `phase`, `stop_requested`, `stop_running`, LED states, `seq_total_ms`).
- Serial format note: one metric per line, `name:value`, baud `9600`.
- Safety note: ISR only sets flags; telemetry emission runs in `loop()`/sequence functions.

### Step 6: Validate and Prepare PR

- Build check: `pio run`.
- Runtime check: verify state and phase transitions in serial output.
- Confirm no functional behavior change in traffic sequence timing.
- Commit in small logical chunks, push branch, then open PR.

Commands:

```bash
pio run
pio device monitor -b 9600

git add src/main.cpp
git commit -m "feat: add telemetry helpers and feature flags"

git add src/main.cpp
git commit -m "feat: emit runtime telemetry across state machine"

git add README.md docs/API.md
git commit -m "docs: add Teleplot and VaporView telemetry guide"

git push -u origin feature/telemetry-teleplot-vaporview
```

Why:

- Small commits make review and rollback easier.
- Pushing the feature branch is the trigger point to open the PR on GitHub.

### 1. Compile-Time Flags

Add flags near top of `src/main.cpp`:

- `ENABLE_TELEMETRY` (default `1`)
- `ENABLE_DEBUG_LOGS` (default `1`) for existing human logs
- Optional `TELEMETRY_HEARTBEAT_MS` (default `250`)

This allows turning telemetry on/off without removing code.

### 2. Output Format

Use simple text metrics compatible with Teleplot and easy for VaporView parsing:

- One metric per line: `name:value`

Examples:

- `state:0`
- `led_green:1`
- `phase:40`
- `seq_total_ms:12480`

### 3. ISR Safety Model

Do not print from ISR.

- ISR sets a lightweight flag, e.g. `volatile bool buttonEdgePending = false;`
- Main loop consumes and emits telemetry pulse safely.

## Signals to Emit

## Core State

- `state`
  - `0` normal
  - `1` transition delay
  - `2` stop sequence
- `stop_requested` (`0/1`)
- `stop_running` (`0/1`)
- `btn_edge` pulse (`1` when seen in loop, then optional `0`)

## Outputs

- `led_green` (`0/1`)
- `led_amber` (`0/1`)
- `led_red` (`0/1`)
- `led_builtin` (`0/1`)
- `buzzer_on` (`0/1`)

## Sequence Progress

- `phase`
  - `10` green off
  - `20` amber x5
  - `30` red on
  - `40` buzzer x20
  - `50` amber x3
  - `60` done
- `seq_ms` elapsed during sequence
- `seq_total_ms` final duration

## Optional Diagnostics

- `wait_ms` heartbeat while waiting for button (throttled)
- `amber_flash_idx` (1..5 / 1..3)
- `beep_idx` (1..20)

## Where to Emit (Code Touch Points)

## `setup()`

- Emit initial baseline once after pin/interrupt setup:
  - `state=0`, output states, flags.

## `loop()`

- After setting normal outputs:
  - emit state and LED flags.
- In waiting loop:
  - emit throttled `wait_ms` heartbeat.
- Before 3-second delay:
  - emit `state=1`, transition marker.
- Before `runStopSequence()`:
  - emit `stop_requested=0` after reset.

## `runStopSequence()`

- At entry:
  - `state=2`, `stop_running=1`, `phase=10`.
- At each phase boundary:
  - update `phase` and relevant outputs.
- During buzzer/amber loops:
  - emit output toggles and optional iteration indices.
- At completion:
  - `seq_total_ms`, `stop_running=0`, `state=0`, `phase=60`.

## `stopTraffic()` ISR

- Only set flags:
  - `stopRequested = true`
  - `buttonEdgePending = true`
- No `Serial.print*` calls in ISR.

## Refactoring Plan (Small and Safe)

1. Add tiny helper functions:
   - `emitMetric(const char* name, long value)`
   - `emitPinState(const char* name, uint8_t pin)`
2. Add small telemetry state variables:
   - `volatile bool buttonEdgePending`
   - `unsigned long lastHeartbeatMs`
3. Insert emit calls at boundaries only.
4. Keep existing log lines unless disabled via flag.

## README Plan

Add a new section: `Telemetry (Teleplot and VaporView)`

Include:

- What telemetry is emitted.
- Serial format and baud (`9600`).
- Quick setup for Teleplot and VaporView.
- Signal dictionary table (`name`, type, meaning, value ranges).
- Sample output snippet.

## API Doc Plan

Update `docs/API.md` with:

- Telemetry signal list.
- Phase code map.
- Safety note about ISR and deferred emission.

## Test and Validation Plan

## Build/Static

- `pio run` passes with no compile errors.
- Verify no warnings from added helpers/macros.

## Runtime Functional

- Normal mode still shows green LED behavior.
- Button press still triggers same stop sequence timing.
- Repeated button presses during sequence still ignored.

## Telemetry Validation

- Confirm expected signal lines in serial monitor.
- Confirm `state` transitions: `0 -> 1 -> 2 -> 0`.
- Confirm `phase` sequence: `10,20,30,40,50,60`.
- Confirm `seq_total_ms` approx 12000-13000 ms.

## Performance/Noise Control

- Confirm waiting-loop heartbeat is throttled (not flooding).
- Confirm no Serial output from ISR path.

## Risks and Mitigations

- Risk: Serial flooding affects timing.
  - Mitigation: throttle heartbeat and emit mostly on transitions.
- Risk: ISR growth causes latency issues.
  - Mitigation: keep ISR flag-only.
- Risk: Tool format mismatch.
  - Mitigation: keep `name:value` baseline and add optional format flag later.

## Acceptance Criteria

- Firmware behavior unchanged functionally.
- Teleplot/VaporView can visualize core runtime signals.
- Docs explain setup and signal semantics clearly.
- Feature is isolated in its own PR with clean commit history.

## Suggested Commit Breakdown

1. `feat: add telemetry helpers and flags`
2. `feat: emit runtime telemetry across state machine`
3. `docs: add Teleplot/VaporView telemetry guide`

## Rollback Plan

If telemetry causes issues:

- Set `ENABLE_TELEMETRY 0` and rebuild.
- Keep code paths compiled but inactive.
- Re-enable after tuning emission frequency.
