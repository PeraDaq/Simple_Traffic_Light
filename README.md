# Simple Traffic Light

![Simple Traffic Light Controller](https://github.com/user-attachments/assets/546e8721-4db4-4334-a672-aa2e9e30e8f3)

**An interrupt-driven traffic light controller built on Arduino UNO with hardware simulation support.**

A simple yet complete traffic light control system that demonstrates real-world embedded systems concepts: hardware interrupts, state machines, timing control, and I/O management. The system operates in normal traffic mode (green light ON) and transitions to a coordinated stop sequence (amber flashing → red light → audible alert) when a push button is pressed.

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Quick Start](#quick-start)
4. [How It Works](#how-it-works)
5. [Project Structure](#project-structure)
6. [Hardware Setup](#hardware-setup)
7. [Building & Uploading](#building--uploading)
8. [Running in Wokwi](#running-in-wokwi)
9. [Debugging](#debugging)
10. [Architecture](#architecture)
11. [Documentation](#documentation)
12. [Contributing](#contributing)
13. [License](#license)
14. [Troubleshooting](#troubleshooting)
15. [FAQ](#faq)

---

## Overview

### What Is This Project?

The Simple Traffic Light is an educational and practical embedded systems project that implements a traffic light controller using an Arduino microcontroller. It's designed to teach and demonstrate:

- **Hardware Interrupts**: Using INT0 for immediate button response
- **State Machines**: Managing system states (normal, transition, stop sequence)
- **Timing Control**: Precise sequencing with delays and millisecond accuracy
- **GPIO Management**: Controlling multiple digital outputs (LEDs, buzzer)
- **Real-time Response**: Non-blocking interrupt handling
- **Serial Debugging**: Monitoring system behavior via serial output

### Who Should Use This?

- 🎓 **Students**: Learning Arduino programming and embedded systems
- 👨‍💻 **Hobbyists**: Building a functional traffic light project
- 🔬 **Educators**: Teaching interrupt-driven programming
- 🚀 **Developers**: Reference implementation for traffic control systems
- 🧪 **Simulators**: Testing in Wokwi before physical deployment

### Project Status

**Version**: 1.0.0  
**Status**: Stable & Production-Ready ✓  
**Last Updated**: 2026-03-04  
**License**: MIT

---

## Features

### Core Functionality

✅ **Interrupt-Driven Design**

- Hardware interrupt (INT0) on pin D2
- Immediate button response (<5µs latency)
- Non-blocking ISR (interrupt service routine)

✅ **Traffic Light States**

- **Normal State**: Green LED continuously ON
- **Transition Delay**: 3-second post-press delay
- **Stop Sequence**: Coordinated amber flash → red → buzzer → amber

✅ **Stop Sequence Details**

- Phase 1: Green turns OFF (500ms)
- Phase 2: Amber flashes 5 times (5 seconds)
- Phase 3: Red turns ON
- Phase 4: Buzzer beeps 20 times at 2000Hz (4 seconds, parallel with red)
- Phase 5: Amber flashes 3 more times (3 seconds)
- Total duration: ~12 seconds

✅ **Audio/Visual Feedback**

- 3 color-coded LEDs (Red, Amber/Yellow, Green)
- Active buzzer with 2000Hz tone generation
- Built-in status LED indicator (green=normal, red=stop)
- Serial debug output at 9600 baud

✅ **Dual Environment Support**

- **PlatformIO**: Cross-platform build system
- **Wokwi**: Browser-based Arduino simulator
- Same firmware runs in both environments

✅ **Safety Features**

- Button presses ignored during stop sequence
- 3-second transition delay for driver/pedestrian reaction time
- Built-in LED status indicator
- Serial logging for real-time monitoring

### Hardware Requirements

**Minimal Bill of Materials**:

- 1× Arduino UNO R3 ($20-25)
- 3× Standard 5mm LEDs ($0.30)
- 3× 220Ω resistors ($0.15)
- 1× Push button ($0.50)
- 1× Active buzzer ($1-3)
- 1× Breadboard ($3-5)
- Jumper wires (~$3-5)

**Total Project Cost**: ~$30-40 USD

---

## Quick Start

### 1-Minute Setup

```bash
# Clone the repository
git clone https://github.com/PeraDaq/Simple_Traffic_Light.git
cd Simple_Traffic_Light

# Install PlatformIO (if not already installed)
pip install platformio

# Build the firmware
pio run

# Upload to Arduino (with board connected)
pio run -t upload

# Monitor serial output
pio device monitor -b 9600
```

### Hardware Assembly

**In 5 steps**:

1. **Connect LEDs**: Red (D5), Amber (D3), Green (D4) with 220Ω resistors
2. **Connect Button**: Push button to D2 (with internal pull-up)
3. **Connect Buzzer**: Buzzer to D6
4. **Common Ground**: All components share GND
5. **Power**: 5V from USB to breadboard power rail

📖 **See [HARDWARE.md](docs/HARDWARE.md) for detailed wiring diagrams and component specifications.**

### First Test

1. Power on Arduino via USB
2. Open serial monitor at 9600 baud
3. You should see:

    ```text
    Setting up Arduino Green, Amber, Red LED Pins (3, 4, 5)...
    Setting up Arduino Push Button "Input Pullup" pin 2...
    Attaching ISR to Push Button pin, monitoring button status continuously!
    Green Traffic Light On!
    ```

4. Press the button - system will execute stop sequence
5. After ~12 seconds, it returns to normal (green ON)

---

## How It Works

### State Flow Diagram

```text
                    ┌─────────────────┐
                    │   Boot/Reset    │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │   setup() runs  │
                    │ Initialize pins │
                    │ Attach ISR      │
                    └────────┬────────┘
                             │
                             ▼
          ╔═════════════════════════════════╗
          ║    NORMAL STATE (Green ON)      ║
          ║  - Green LED = ON               ║
          ║  - Amber LED = OFF              ║
          ║  - Red LED = OFF                ║
          ║  - Waiting for button press     ║
          ║  - While loop blocking          ║
          ╚════════════┬══════════════════╝
                       │
        ┌──────────────┤
        │ Button press │ [ISR triggered]
        │ (FALLING)    │ [stopRequested = true]
        └──────────��───┤
                       │
                       ▼
          ┌─────────────────────────────┐
          │  TRANSITION DELAY           │
          │  (3-second pause)           │
          │  delay(3000)                │
          │  Allow driver reaction time │
          └────────┬────────────────────┘
                   │
                   ▼
          ╔═════════════════════════════════╗
          ║   STOP SEQUENCE EXECUTING       ║
          ║  stopSequenceRunning = true     ║
          ║                                 ║
          ║  Phase 1: Green OFF (500ms)     ║
          ║  Phase 2: Amber flash 5× (5s)   ║
          ║  Phase 3: Red ON                ║
          ║  Phase 4: Buzzer 20× (4s)       ║
          ║  Phase 5: Amber flash 3× (3s)   ║
          ║                                 ║
          ║  Duration: ~12 seconds          ║
          ║  Button presses IGNORED         ║
          ╚═════════════╬══════════════════╝
                        │
                        ▼
          ┌─────────────────────────────┐
          │  Sequence Complete          │
          │  stopSequenceRunning = false│
          │  stopRequested = false      │
          │  Reset to normal state      │
          └────────┬────────────────────┘
                   │
                   └──────────┬──────────┘
                              │
                    Back to NORMAL STATE ↺
```

### Key Concepts Explained

#### 1. **Hardware Interrupt (INT0 on D2)**

```cpp
// Button pressing triggers ISR immediately
attachInterrupt(digitalPinToInterrupt(push_button), stopTraffic, FALLING);
```

**Why Use Interrupts?**

- ✓ Immediate response to button press
- ✓ Don't miss presses even if main code is busy
- ✓ Cleaner than polling in main loop
- ✓ Professional embedded systems practice

#### 2. **State Machine with Volatile Flags**

```cpp
volatile bool stopRequested = false;        // Button pressed?
volatile bool stopSequenceRunning = false;  // Sequence executing?
```

**Why Volatile?**

- Flags can change in ISR
- Tells compiler not to optimize away flag checks
- Ensures CPU reads actual RAM value, not cached register

#### 3. **Non-blocking ISR**

```cpp
void stopTraffic() {
  if (!stopSequenceRunning) {
    stopRequested = true;  // Just set flag (fast ~1µs)
  }
}
// Never does: delay(), Serial.println(), digitalWrite()
// Those take too long (µs vs ms)
```

**Why Keep ISR Short?**

- Blocks other code from executing
- Longer ISR = missed interrupts
- Real-time systems require fast ISRs

#### 4. **Sequence Timing**

```text
Timeline:     0s    3s     3.5s  5.5s  9.5s  12.5s
              │     │      │     │     │     │
Button ───────┘     │      │     │     │     │
       press        3s     │     │     │     │
                   wait    │     │     │     │
                          ├─────────────────┤
                         Sequence runs    End
                         (~12 seconds)
```

---

## Project Structure

```text
Simple_Traffic_Light/
│
├── README.md                 # This file - Complete project guide
├── platformio.ini           # PlatformIO configuration
├── .gitignore              # Git ignore file
│
├── docs/
│   ├── HARDWARE.md          # Hardware specifications and wiring
│   ├── API.md               # Complete API reference
│   ├── CONTRIBUTING.md      # Contribution guidelines
│   ├── CHANGELOG.md         # Version history
│   ├── LICENSE.md           # MIT License
│   └── WORKLOG.md           # Development log
│
├── src/
│   └── main.cpp            # Main firmware (PlatformIO)
│
└── Wokwi/
    ├── sketch.ino          # Wokwi simulation firmware
    ├── diagram.json        # Circuit diagram (Wokwi format)
    └── wokwi.toml          # Wokwi configuration
```

### File Descriptions

| File | Purpose | For |
| --- | --- | --- |
| README.md | Project overview & setup | Everyone |
| docs/HARDWARE.md | Pin configs, wiring, schematics | Hardware builders |
| docs/API.md | Function reference, timing specs | Programmers |
| docs/CONTRIBUTING.md | Development guidelines | Contributors |
| docs/CHANGELOG.md | Version history, features | Users & maintainers |
| docs/LICENSE.md | License terms | Everyone |
| docs/WORKLOG.md | Development log | Maintainers |
| platformio.ini | Build configuration | PlatformIO users |
| src/main.cpp | Main firmware code | Arduino developers |
| Wokwi/* | Simulation files | Simulator users |

---

## Hardware Setup

### Quick Hardware Checklist

**Components**:

- [ ] Arduino UNO R3
- [ ] 3× Standard 5mm LEDs (Red, Amber, Green)
- [ ] 3× 220Ω resistors
- [ ] Push button (momentary NO)
- [ ] Active buzzer (5V)
- [ ] Breadboard
- [ ] Jumper wires
- [ ] USB cable (Micro-B)

**Connections**:

- [ ] Green LED anode → D4 via 220Ω resistor, cathode → GND
- [ ] Amber LED anode → D3 via 220Ω resistor, cathode → GND
- [ ] Red LED anode → D5 via 220Ω resistor, cathode → GND
- [ ] Button left → 5V, right → D2, right → GND
- [ ] Buzzer (+) → D6, (-) → GND
- [ ] Arduino 5V → 5V rail, GND → GND rail

**Verification**:

- [ ] All connections firm in breadboard
- [ ] No shorted wires
- [ ] LED polarity correct (longer lead = positive)
- [ ] 220Ω resistors in series with LEDs
- [ ] Button connected to both 5V and GND (via button contact)
- [ ] Buzzer polarity correct

📖 **For detailed schematic diagrams, see [HARDWARE.md](docs/HARDWARE.md)**

---

## Building & Uploading

### Build Only (No Upload)

```bash
# Compile firmware (no upload)
pio run

# Expected output:
# Compiling .pio/build/uno/src/main.cpp.o
# Linking .pio/build/uno/firmware.elf
# Building .pio/build/uno/firmware.hex
# ✓ SUCCESS
```

### Build & Upload to Hardware

```bash
# Full build and upload
pio run -t upload

# With explicit serial port (if auto-detect fails)
pio run -t upload --upload-port /dev/ttyUSB0  # Linux
pio run -t upload --upload-port COM3           # Windows
pio run -t upload --upload-port /dev/cu.usbserial # macOS
```

### Monitor Serial Output

```bash
# Start serial monitor at 9600 baud
pio device monitor -b 9600

# Expected output:
# Setting up Arduino Green, Amber, Red LED Pins (3, 4, 5)...
# Setting up Arduino Push Button "Input Pullup" pin 2...
# Attaching ISR to Push Button pin, monitoring button status continuously!
# Green Traffic Light On!
# Doing nothing .. :)..
# Doing nothing .. :)..
# [etc - continues until button pressed]

# Exit monitor: Ctrl+C
```

### Troubleshooting Build Issues

#### Error: Board not found

```bash
# List available boards
pio boards | grep uno

# Specify board explicitly
pio run --environment uno
```

#### Error: Permission denied (Linux)

```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in
```

#### Error: "The system cannot find the path specified" (Windows)

```bash
# Ensure PlatformIO is in PATH
# Try restarting IDE or command prompt
```

---

## Running in Wokwi

### Browser-Based Simulation

Wokwi is a browser-based Arduino simulator. No hardware needed!

**Advantages**:

- ✅ Test before buying hardware
- ✅ No USB cable needed
- ✅ Fast iteration and debugging
- ✅ Visual circuit validation

### Step-by-Step Setup

#### 1. Build Firmware

```bash
cd Simple_Traffic_Light
pio run
# Generates: .pio/build/uno/firmware.hex
```

#### 2. Open in Wokwi

```bash
# Option A: Open Wokwi/diagram.json in browser
# Go to https://wokwi.com and open the file
# OR load project directly from Wokwi

# Option B: Check wokwi.toml configuration
cat Wokwi/wokwi.toml
# Verify firmware paths point to generated .hex and .elf files
```

#### 3. Verify Configuration (wokwi.toml)

```toml
[wokwi]
version = 1
author = "Mohanad Sharif"
title = "Simple Traffic Light"
board = "uno"
firmware = "../.pio/build/uno/firmware.hex"
elf = "../.pio/build/uno/firmware.elf"
```

**Path Notes**:

- Paths are relative to `Wokwi/` directory
- `../` goes up one level to project root
- `.pio/build/uno/` is PlatformIO build output

#### 4. Run Simulation

1. Click "Play" button in Wokwi
2. Green LED lights up immediately
3. Click simulated push button
4. Observe:
   - 3-second delay (visible in serial output)
   - Amber LED flashes
   - Red LED turns on
   - Buzzer beeps (if audio enabled in browser)
   - Amber flashes again
   - Return to green after ~12 seconds

#### 5. Debug in Wokwi

- **Serial Output**: View in Wokwi console panel
- **Logic Analyzer**: Included in Wokwi diagram to monitor pin states
- **Breakpoints**: Not available in Wokwi, use serial debugging instead

### Wokwi Simulator Components

The Wokwi circuit includes:

```json
{
  "parts": [
    {
      "type": "wokwi-arduino-uno",
      "description": "Microcontroller"
    },
    {
      "type": "wokwi-led",
      "color": "red",
      "description": "Red LED"
    },
    {
      "type": "wokwi-led",
      "color": "orange",
      "description": "Amber LED"
    },
    {
      "type": "wokwi-led",
      "color": "green",
      "description": "Green LED"
    },
    {
      "type": "wokwi-resistor",
      "value": "220",
      "description": "220Ω current-limiting resistors (×3)"
    },
    {
      "type": "wokwi-pushbutton",
      "description": "Push button"
    },
    {
      "type": "wokwi-buzzer",
      "description": "Active buzzer"
    },
    {
      "type": "wokwi-logic-analyzer",
      "description": "Pin state analyzer (debug)"
    }
  ]
}
```

### Common Wokwi Issues & Fixes

**Issue**: Firmware doesn't update in simulation  
**Fix**: Rebuild with `pio run`, hard refresh browser (Ctrl+Shift+R), restart simulation

**Issue**: Buzzer is silent  
**Fix**:

1. Check browser audio permissions
2. Increase buzzer volume in diagram.json: `"volume": "0.5"`
3. Click on browser to enable audio (user interaction required)

**Issue**: LEDs don't light  
**Fix**: Verify `diagram.json` has LED connections, ensure firmware path is correct

---

## Debugging

### Serial Monitor Debugging

The firmware outputs detailed debug information:

```bash
# Open serial monitor
pio device monitor -b 9600

# Watch for these key messages:
# Startup: "Setting up Arduino..." "Attaching ISR..."
# Normal: "Green Traffic Light On!" "Doing nothing .. :).."
# Button: "Button Pressed! Waiting..."
# Sequence: "Green Off", "Flashing Amber", "Red On", "Buzzer On/Off"
# Complete: "Total time taken for stop sequence: X seconds"
```

### Analyzing Serial Output

**Expected Output (Normal Operation)**:

```text
Green Traffic Light On!
Doing nothing .. :)..
Doing nothing .. :)..
Doing nothing .. :)..
[continues until button pressed]
```

**Expected Output (After Button Press)**:

```text
Button Pressed! Waiting for 3 seconds before running stop sequence...
[3-second delay - no output]
Button pressed! ISR Flag toggled to True!
ISR Flag toggled to False! Running Stop Sequence...
Green Off
Flashing Amber On.....
Flashing Amber OFF.....
Red On
Buzzer On
Buzzer Off
Flashing Amber On
Flashing Amber Off.....
Red Off.....
Total time taken for stop sequence: 12 seconds
Green Traffic Light On!
Doing nothing .. :)..
```

### Common Debug Scenarios

#### Scenario 1: Button doesn't respond

- Check serial output shows "Green Traffic Light On!"
- If not, hardware isn't responding
- Verify Button connection to D2
- Use multimeter to check pin voltage

#### Scenario 2: Sequence runs but LEDs don't light

- Verify LED connections (polarity, resistors)
- Check serial shows sequence executing
- Use multimeter to verify voltage at LED pins
- LEDs may be reversed (anode/cathode swapped)

#### Scenario 3: Buzzer doesn't sound

- Hardware: Check D6 voltage (should pulse at 2000Hz)
- Wokwi: Enable browser audio, increase volume in diagram.json
- Verify `tone()` or `digitalWrite()` is being called

---

## Architecture

### Software Architecture

```text
┌─────────────────────────────────────────┐
│         Arduino Firmware                │
│  (src/main.cpp or Wokwi/sketch.ino)    │
└──────────────┬──────────────────────────┘
               │
        ┌──────┴──────┐
        │             │
        ▼             ▼
   ┌────────┐   ┌──────────┐
   │ setup()│   │  loop()  │
   └────────┘   └──────────┘
        │             │
        ├─ Init pins  ├─ Normal state (green ON)
        ├─ Init serial├─ Wait for button
        └─ Attach ISR ├─ 3s transition delay
                      ├─ Call runStopSequence()
                      └─ Reset flags
                            │
                            ▼
                      ┌──────────────────┐
                      │runStopSequence() │
                      │                  │
                      ├─ Phase 1: Green  │
                      ├─ Phase 2: Amber  │
                      ├─ Phase 3: Red    │
                      ├─ Phase 4: Buzzer │
                      ├─ Phase 5: Amber  │
                      └─ Measure time    │
                            │
                            └─ Back to loop()

        ┌────────────────────────────┐
        │  ISR (stopTraffic)         │
        │  [Interrupt Handler]       │
        │                            │
        ├─ Triggered by button press │
        ├─ Sets stopRequested flag   │
        └─ Returns immediately       │
```

### Data Flow

```text
Hardware Event          ISR               Main Loop
───────────────        ───               ─────────
Button press ──→  stopTraffic() ──→  stopRequested flag ──→ loop()
                  Set flag (1µs)      checked (normal)   │
                  Return (fast)       detects change     │
                                                         ▼
                                                   Execute sequence
```

### State Transitions

```cpp
// Current state determined by flags:

if (stopSequenceRunning) {
  // Inside stop sequence
  // Block main loop until complete
  // Ignore button presses
}

else if (stopRequested) {
  // Button was pressed, waiting to execute
  // In transition delay phase
  // About to start sequence
}

else {
  // Normal operation
  // Green light on
  // Waiting for button
}
```

---

## Documentation

### Complete Documentation Set

| Document | Purpose | Read Time |
| --- | --- | --- |
| **README.md** (this file) | Overview, setup, quick start | 10 min |
| **docs/HARDWARE.md** | Wiring, schematics, components | 15 min |
| **docs/API.md** | Function reference, timing specs | 20 min |
| **docs/CONTRIBUTING.md** | Development guidelines | 10 min |
| **docs/CHANGELOG.md** | Version history, features | 5 min |

### Learning Path

**Beginner** (Just want to build it):

1. Read README.md (this file) - 10 min
2. Follow Hardware Setup section
3. Run Quick Start commands
4. Test with button press

**Intermediate** (Want to understand it):

1. Read all of README.md - 30 min
2. Read docs/HARDWARE.md for circuit details
3. Review code structure in src/main.cpp
4. Study docs/CHANGELOG.md for design decisions

**Advanced** (Want to modify/extend it):

1. Read docs/API.md for complete function reference
2. Study interrupt handling and timing
3. Review docs/CONTRIBUTING.md for code guidelines
4. Plan modifications with state machine diagram

---

## Contributing

### How to Contribute

We welcome contributions! See **[CONTRIBUTING.md](docs/CONTRIBUTING.md)** for:

- Development setup
- Code style guidelines
- Testing procedures
- Pull request process
- Reporting issues

### Quick Contribution Process

```bash
# 1. Fork on GitHub
# 2. Clone your fork
git clone https://github.com/YOUR_USERNAME/Simple_Traffic_Light.git
cd Simple_Traffic_Light

# 3. Create feature branch
git checkout -b feature/your-feature

# 4. Make changes
vim src/main.cpp

# 5. Build and test
pio run
pio run -t upload

# 6. Commit with clear message
git commit -m "Add your feature description"

# 7. Push and create pull request
git push origin feature/your-feature
```

### Areas for Contribution

- 🐛 **Bug Fixes**: Fix issues in code
- ✨ **Features**: Add debouncing, configurable timing, etc.
- 📚 **Documentation**: Improve guides and examples
- 🧪 **Testing**: Add test cases
- 🎨 **Improvements**: Code cleanup, optimization

---

## License

This project is licensed under the **MIT License**.

```text
MIT License

Copyright (c) 2026 Mohanad Sharif

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions...

[See LICENSE.md for full text]
```

---

## Troubleshooting

### Common Issues & Solutions

#### Build Issues

**Q: "pio: command not found"**  
**A**: PlatformIO not installed or not in PATH

```bash
pip install platformio
# Add to PATH if needed
```

**Q: "Board not found"**  
**A**: Arduino not connected or wrong driver

```bash
# Check if Arduino appears in device list
pio boards list all | grep uno

# Try explicit port
pio run -t upload --upload-port /dev/ttyUSB0
```

**Q: "Compilation errors"**  
**A**: Check error message for missing files or syntax errors

```bash
# Clean rebuild
pio run --target clean
pio run
```

#### Hardware Issues

**Q: LEDs don't light**  
**A**: Check connections and polarity

- Verify resistor in series with LED
- Check LED anode (long lead) connected to pin via resistor
- Check LED cathode (short lead) connected to GND
- Use multimeter to verify voltage at pins

**Q: Button doesn't work**  
**A**: Check D2 connection and pull-up configuration

- Verify button connected to D2
- Verify `pinMode(push_button, INPUT_PULLUP)` in code
- Use multimeter: should read ~5V when released, 0V when pressed
- Serial should show "Waiting for button" message

**Q: Buzzer is silent**  
**A**: Verify D6 connection and tone frequency

- Check buzzer connected to D6
- Listen closely (2000Hz is relatively quiet)
- Verify `tone(buzzer, 2000)` is called
- Use oscilloscope or multimeter to verify 2000Hz signal

**Q: Serial output is garbage**  
**A**: Wrong baud rate or COM port

- Set serial monitor to **9600 baud**
- Verify correct COM port selected
- Try different cable if available

#### Wokwi Issues

**Q: Firmware doesn't load in Wokwi**  
**A**: Rebuild and verify paths

```bash
pio run                    # Rebuild firmware
# Check Wokwi/wokwi.toml firmware paths
# Hard refresh browser: Ctrl+Shift+R
```

**Q: LEDs don't light in simulation**  
**A**: Check diagram.json connections

- Verify LED parts in diagram.json
- Check connections to Arduino pins D3, D4, D5
- Ensure GND connections complete

**Q: Buzzer silent in Wokwi**  
**A**: Enable audio in browser

- Click on page to enable audio (browser requirement)
- Increase buzzer volume in diagram.json: `"volume": "0.5"`
- Check browser audio isn't muted

---

## FAQ

### General Questions

**Q: Is this suitable for beginners?**  
A: Yes! It's designed as an educational project. Start with Quick Start section and follow along. See [CONTRIBUTING.md](docs/CONTRIBUTING.md) for learning resources.

**Q: Can I modify the timing?**  
A: Absolutely! The code uses `delay()` statements. You can change 3000 to 2000 for a 2-second delay, for example. See [API.md](docs/API.md) for timing details.

**Q: Does this work on other Arduino boards?**  
A: Yes! Arduino Nano, Pro Mini, and Mega are compatible. You may need to adjust pin assignments. See [API.md](docs/API.md) compatibility section.

**Q: Can I add more buttons?**  
A: Yes! You would need to use INT1 (pin 3) for a second interrupt. See interrupt-driven design patterns in embedded systems references.

### Technical Questions

**Q: Why use hardware interrupts instead of polling?**  
A: Interrupts provide immediate response without blocking the main loop. Professional embedded systems use this pattern.

**Q: Why is the stop sequence 12 seconds?**  
A: This is a realistic traffic light duration. Real-world traffic lights typically have 10-15 second amber/red phases. Adjust to your needs!

**Q: Can I use PWM to fade LEDs?**  
A: Yes! Change `digitalWrite(led, HIGH)` to `analogWrite(led, 128)` for brightness control. Pins D3, D5 support PWM.

**Q: How does the buzzer work?**  
A: Using the `tone()` function, which generates a PWM signal at 2000Hz. The buzzer converts this to audible sound.

**Q: Can I measure power consumption?**  
A: Yes, connect a multimeter in series with 5V supply. Peak is ~110mA (all LEDs + buzzer on). USB provides 500mA, so you're safe.

### Project Questions

**Q: How do I deploy this to production?**  
A: This is an educational project. For real traffic lights, you'd need:

- More powerful controller (typically PLC or industrial computer)
- Redundancy and safety systems
- Regulatory compliance (IEC 12923 standard)
- Weather-resistant enclosure
- Additional safety devices

**Q: Can I add pedestrian crossing mode?**  
A: Yes! This would be a great feature to add. See [CONTRIBUTING.md](docs/CONTRIBUTING.md) for contribution guidelines.

**Q: Where can I learn more?**  
A:

- Arduino documentation: [https://www.arduino.cc](https://www.arduino.cc)
- Interrupts: [https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)
- State machines: Search "Arduino state machine tutorial"

---

## Support and Contact

### Getting Help

1. **Check Documentation**: README (you are here), docs/HARDWARE.md, docs/API.md
2. **Search Issues**: [https://github.com/PeraDaq/Simple_Traffic_Light/issues](https://github.com/PeraDaq/Simple_Traffic_Light/issues)
3. **Read Troubleshooting**: See Troubleshooting section above
4. **Open Issue**: Report bug or request feature on GitHub

### Reporting Issues

When reporting a bug, include:

- Arduino board and IDE/PlatformIO version
- Exact error message or symptom
- Steps to reproduce
- Relevant code or configuration
- Hardware setup (photos helpful)
- Serial output logs

### Feature Requests

When requesting a feature:

- Describe the desired behavior
- Explain the use case
- Provide examples if possible
- Suggest implementation approach (if you have ideas)

---

## Project Statistics

| Metric | Value |
| --- | --- |
| Lines of Code | ~150 |
| Flash Memory Used | 20.8% |
| RAM Memory Used | 35.9% |
| Build Time | <2 seconds |
| Upload Time | <5 seconds |
| Supported Boards | 5+ (UNO, Nano, Mega, etc) |
| Dependencies | 0 (uses Arduino core) |
| Code Complexity | Low (perfect for learning) |

---

## Project Roadmap

### Completed (v1.0)

✅ Basic traffic light functionality  
✅ Hardware interrupt handling  
✅ Stop sequence with timing  
✅ Wokwi simulation support  
✅ Complete documentation  

### Planned (Future Versions)

- [ ] Debounce logic for button
- [ ] Configurable timing parameters
- [ ] Multiple button support
- [ ] Pedestrian crossing mode
- [ ] Additional test cases

---

## Additional Resources

### Learning Materials

- [Arduino Interrupts Tutorial](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)
- [State Machine Design Patterns](https://en.wikipedia.org/wiki/Finite-state_machine)
- [Embedded Systems Design](https://www.embedded.com/)

### Tools & Simulators

- [Wokwi - Arduino Simulator](https://wokwi.com)
- [PlatformIO - Build System](https://platformio.org)
- [Arduino IDE](https://www.arduino.cc/en/software)

### Standards & References

- [Traffic Light Standards](https://en.wikipedia.org/wiki/Traffic_light)
- [Real-time Systems](https://en.wikipedia.org/wiki/Real-time_computing)
- [Arduino Safety](https://www.arduino.cc/en/Guide/Safety)

---

## Acknowledgments

### Built With

- **Arduino Framework**: Core microcontroller support
- **PlatformIO**: Build system and project management
- **Wokwi**: Simulation platform
- **Community**: Feedback and contributions

### Thanks To

- Arduino community for excellent documentation
- Wokwi team for browser-based simulator
- Contributors and testers
- You for checking out this project!

---

## Version History

| Version | Date | Status | Notes |
| --- | --- | --- | --- |
| 1.0.0 | 2026-03-04 | Stable | Initial release, fully tested |

See [CHANGELOG.md](docs/CHANGELOG.md) for detailed version history.

---

## Checklist: Before Using This Project

- [ ] Read this entire README
- [ ] Review [HARDWARE.md](docs/HARDWARE.md) for wiring
- [ ] Gather required components
- [ ] Install PlatformIO or Arduino IDE
- [ ] Clone/download the repository
- [ ] Run `pio run` to verify build succeeds
- [ ] Assemble hardware
- [ ] Upload firmware
- [ ] Test with button press
- [ ] Celebrate success! 🎉

---

Built with ❤️ for learning and embedded systems education

[⬆ Back to top](#simple-traffic-light) | [Report Issue](https://github.com/PeraDaq/Simple_Traffic_Light/issues) | [Contribute](docs/CONTRIBUTING.md)

[MIT License](docs/LICENSE.md) © 2026 Mohanad Sharif
