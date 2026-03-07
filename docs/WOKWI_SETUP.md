# Wokwi Setup Guide

This guide explains how to run, verify, and troubleshoot the `Simple_Traffic_Light` simulation in Wokwi using the current project layout.

## Scope

This guide covers:

- Project-side setup (`Wokwi/diagram.json`, `Wokwi/wokwi.toml`)
- Firmware build and sync from PlatformIO
- Running simulation in browser or VS Code
- Serial and logic-analyzer checks
- Common issues and fixes

## Project Layout

Relevant files in this repository:

```text
Simple_Traffic_Light/
  README.md
  platformio.ini
  src/main.cpp
  Wokwi/
    diagram.json
    wokwi.toml
    sketch.ino
```

Notes:

- PlatformIO firmware is built from `src/main.cpp`.
- Wokwi can run either source-based simulation (`sketch.ino`) or prebuilt firmware (`.hex`/`.elf`) depending on configuration.
- This project's `wokwi.toml` currently points to PlatformIO build artifacts.

## Prerequisites

- PlatformIO CLI installed and working
- A successful local build with `pio run`
- Wokwi account (for cloud workflows)
- Browser audio enabled (for buzzer validation)

Optional (VS Code):

- Wokwi extension
- PlatformIO extension

## 1. Build Firmware Artifacts

From project root:

```bash
pio run
```

Expected output files:

- `.pio/build/uno/firmware.hex`
- `.pio/build/uno/firmware.elf`

If these files are missing, Wokwi firmware-path mode will not start.

## 2. Verify `Wokwi/wokwi.toml`

Current file in this repository:

```toml
[wokwi]
version = 1
author = "Mohanad Sharif"
title = "Simple Traffic Light"
description = "Interrupt-driven traffic light with LEDs, push button, and buzzer"
board = "uno"
firmware = '../.pio/build/uno/firmware.hex'
elf = '../.pio/build/uno/firmware.elf'
```

What matters technically:

- `board = "uno"` matches the project target.
- `firmware` and `elf` paths are relative to `Wokwi/` and point to PlatformIO outputs.

## 3. Verify Circuit in `Wokwi/diagram.json`

This project includes:

- Arduino Uno (`wokwi-arduino-uno`)
- 3 LEDs (red/orange/green) + 220 ohm resistors
- Push button on D2
- Buzzer on D6
- Logic analyzer connected to D2, D3, D4, D5, D6

Pin expectations for this firmware:

- D2: push button (interrupt input)
- D3: amber LED
- D4: green LED
- D5: red LED
- D6: buzzer

Audio note:

- `diagram.json` currently sets buzzer `"volume": "0.1"`.
- If sound is too quiet, increase to `0.5` or `1`.

## 4. Run Simulation

### Option A: Wokwi Web

1. Open Wokwi project or import from local files.
2. Ensure `diagram.json` and `wokwi.toml` are loaded.
3. Click Run/Play.
4. Confirm startup behavior:
   - Green LED on in normal mode
   - Button press triggers sequence after 3 seconds

### Option B: VS Code + Wokwi Extension

1. Open `Wokwi/diagram.json`.
2. Start simulation with the extension command.
3. If file opens as simulator but you need raw JSON edits, use:
   - `Reopen Editor With...` -> `Text Editor`

## 5. Expected Runtime Behavior

On startup:

- Green LED ON
- Amber and Red OFF
- Button waits on D2 interrupt

After button press:

1. 3-second delay
2. Green OFF
3. Amber flashes 5 times
4. Red ON
5. Buzzer beeps (tone cycle on D6)
6. Amber flashes 3 times
7. Return to normal mode

## 6. Validation Checklist

- Build succeeds with `pio run`
- Firmware files exist in `.pio/build/uno/`
- `wokwi.toml` paths are valid
- LEDs match expected pin behavior
- Buzzer audible
- Logic analyzer toggles on D2/D3/D4/D5/D6 during sequence

## 7. Logic Analyzer Workflow

The diagram includes a logic analyzer (`logic1`) wired to key pins.

Use it to confirm:

- D2 edge when button is pressed
- D3 pulse train for amber flashes
- D5 high interval for red phase
- D6 pulse/tone activity during buzzer phase

This is useful when LED or buzzer behavior appears inconsistent.

## 8. Common Problems and Fixes

### Simulation starts but uses old behavior

Fix:

```bash
pio run
```

Then restart simulation and refresh browser tab.

### No buzzer sound

Checks:

- Browser/tab not muted
- User interaction performed (some browsers require click to enable audio)
- Buzzer volume in `diagram.json` increased
- D6 connection intact in `connections`

### LEDs do not match expected order

Checks:

- Confirm pins in `src/main.cpp` match `diagram.json` wiring
- Verify no stale firmware from previous build

### Firmware path error

Checks:

- `wokwi.toml` path points to existing `.hex/.elf`
- You ran `pio run` before launching simulation

## 9. Recommended Workflow for This Repo

Use this repeatable cycle:

1. Edit `src/main.cpp`
2. Run `pio run`
3. Launch/restart Wokwi simulation
4. Validate serial output + logic analyzer + LED/buzzer behavior

This keeps PlatformIO firmware and Wokwi runtime in sync.

## 10. Optional Improvements

- Align `Wokwi/wokwi.toml` title/description with this project name.
- Raise default buzzer volume in `diagram.json` for easier testing.
- Add this guide to README documentation links.
