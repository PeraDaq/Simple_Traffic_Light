# Simple Traffic Light (Arduino UNO + PlatformIO + Wokwi)

<img width="1134" height="782" alt="snip" src="https://github.com/user-attachments/assets/546e8721-4db4-4334-a672-aa2e9e30e8f3" />

A simple interrupt-driven traffic light controller built on **Arduino UNO**.
The system stays in normal traffic mode (green ON), and when the push button is pressed it executes a full stop sequence with amber flashing, red light, and buzzer beeps, then returns to normal operation.

## Getting Started

### Prerequisites
- Arduino IDE or PlatformIO CLI
- Arduino Uno board or compatible microcontroller
- Hardware: 3x LEDs (Red, Green, Amber), Buzzer, Push Button, resistors, breadboard, jumper wires
- Wokwi.com account (for simulation)

### Quick Start
1. Clone the repository
2. Install PlatformIO: `pip install platformio`
3. Build and upload: `pio run -t upload`
4. Monitor output: `pio device monitor -b 9600`

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

## How It Works

The traffic light system uses a **hardware interrupt** (ISR) on pin D2 to detect button presses. When triggered:
1. System waits 3 seconds (transition delay)
2. Executes stop sequence:
   - Green LED turns off
   - Amber LED flashes 5 times (500ms intervals)
   - Red LED turns on
   - Buzzer emits 20 beeps at 2000Hz (100ms intervals)
   - Amber LED flashes 3 more times
3. Returns to normal operation automatically

## Project Details

### Code Architecture
- **setup()**: Initializes pins and attaches ISR to pin D2 (FALLING edge)
- **loop()**: Manages normal state (green ON) and waits for interrupt
- **stopTraffic()**: ISR handler that sets `stopRequested` flag
- **runStopSequence()**: Executes complete stop sequence with timing
- **Built-in LED Indicator**: LOW=normal (green), HIGH=stop mode (red)

### Timing Details
- **Transition Delay**: 3000ms (3 seconds after button press before sequence starts)
- **Amber Flash**: 500ms ON + 500ms OFF (5 flashes before red, 3 after)
- **Buzzer Beeps**: 2000Hz tone for 100ms, 100ms silence (20 beeps total)
- **Total Stop Sequence Duration**: ~10-12 seconds (logged to serial)

### Serial Debug Output
Connect to serial port at 9600 baud to see:
- Pin initialization messages
- State transitions ("Green Traffic Light On!", "Button Pressed!", etc.)
- Timing information ("Total time taken for stop sequence: X seconds")

## Hardware / Pin Mapping

| Component | Arduino Pin | Function |
|---|---|---|
| Green LED | `D4` | Normal traffic state |
| Amber LED | `D3` | Stop sequence flash |
| Red LED | `D5` | Stop/warning state |
| Push Button (INPUT_PULLUP) | `D2` | Interrupt trigger |
| Buzzer | `D6` | Audible alert (2000Hz) |
| Built-in LED | `LED_BUILTIN` | Status indicator |

## Behavior Flow

1. System boots in **normal state** (Green ON).
2. Waits for button press interrupt.
3. On button press, waits **3 seconds**.
4. Runs stop sequence:
   - Green turns OFF
   - Amber flashes 5 times
   - Red turns ON
   - Buzzer beeps 20 times
   - Amber flashes 3 more times
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

## Debugging Tips
- Use serial monitor to trace state changes
- Built-in LED indicates operational state (LOW=normal, HIGH=stop)
- Verify all pins are correctly defined in `#define` statements
- Check buzzer volume setting in `Wokwi/diagram.json` if no sound in simulation

## Notes

- The push button is configured as `INPUT_PULLUP`, so logic is active-low.
- If buzzer sound is too low in simulation, increase buzzer `volume` in `Wokwi/diagram.json`.
- Serial output occurs every 250ms during normal operation (may cause lag).
- Button presses during stop sequence are ignored (safety feature).

## Documentation

Comprehensive project documentation is available in the following files:

- **[CONTRIBUTING.md](CONTRIBUTING.md)** - Development setup, code guidelines, testing checklist, and pull request process
- **[CHANGELOG.md](CHANGELOG.md)** - Version history, release notes, technical specifications, and known issues
- **[LICENSE.md](LICENSE.md)** - MIT License details and terms of use
- **[API.md](API.md)** - Complete API reference, function documentation, GPIO pins, timing specifications, and state machine details

## Author

- Mohanad Sharif