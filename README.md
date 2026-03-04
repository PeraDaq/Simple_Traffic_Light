# Simple Traffic Light (Arduino UNO + PlatformIO + Wokwi)

<img width="1134" height="782" alt="snip" src="https://github.com/user-attachments/assets/546e8721-4db4-4334-a672-aa2e9e30e8f3" />


A simple interrupt-driven traffic light controller built on **Arduino UNO**.
The system stays in normal traffic mode (green ON), and when the push button is pressed it executes a full stop sequence with amber flashing, red light, and buzzer beeps, then returns to normal operation.

## Features

- Normal state: **Green ON**, Amber/Red OFF
- Push-button trigger using **hardware interrupt** (`attachInterrupt` on pin `2`)
- Delayed transition after button press (3 seconds)
- Stop sequence with:
  - Green OFF
  - Amber flashing
  - Red ON
  - Audible buzzer beeps using `tone()` / `noTone()`
- Serial debug logs for behavior tracing
- Works in both:
  - PlatformIO firmware (`src/main.cpp`)
  - Wokwi simulation setup (`Wokwi/`)

## Hardware / Pin Mapping

| Component | Arduino Pin |
|---|---|
| Amber LED | `D3` |
| Green LED | `D4` |
| Red LED | `D5` |
| Push Button (INPUT_PULLUP) | `D2` |
| Buzzer | `D6` |
| Built-in LED | `LED_BUILTIN` |

## Behavior Flow

1. System boots in **normal state** (Green ON).
2. Waits for button press interrupt.
3. On button press, waits **3 seconds**.
4. Runs stop sequence:
   - Green turns OFF
   - Amber flashes
   - Red turns ON
   - Buzzer beeps
   - Amber flashes again
5. Returns to normal state automatically.

## Project Structure

```text
platformio.ini
WORKLOG.md
src/
  main.cpp
Wokwi/
  diagram.json
  sketch.ino
  wokwi.toml
```

## Build and Upload (PlatformIO)

From project root:

```bash
pio run
pio run -t upload
pio device monitor -b 9600
```

## Run in Wokwi

- Open the `Wokwi/` simulation project in Wokwi.
- Ensure firmware paths in `Wokwi/wokwi.toml` point to PlatformIO outputs:
  - `.pio/build/uno/firmware.hex`
  - `.pio/build/uno/firmware.elf`
- Start simulation and press the push button to trigger stop mode.

## Notes

- The push button is configured as `INPUT_PULLUP`, so logic is active-low.
- If buzzer sound is too low in simulation, increase buzzer `volume` in `Wokwi/diagram.json`.

## Author

- Mohanad Sharif
