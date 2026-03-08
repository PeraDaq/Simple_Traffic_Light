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
