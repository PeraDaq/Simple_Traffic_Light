# Hardware Guide - Simple Traffic Light

Complete hardware documentation for the Simple Traffic Light interrupt-driven controller, including schematics, component specifications, wiring diagrams, and troubleshooting guides.

---

## 📦 Bill of Materials (BOM)

### Microcontroller

| Item | Qty | Spec | Purpose | Cost |
|------|-----|------|---------|------|
| Arduino Uno R3 | 1 | ATmega328P @ 16MHz | Main controller | $20-25 |
| USB Cable | 1 | USB-A to Micro-B | Power & Programming | $3-5 |

### LEDs (Traffic Lights)

| Item | Qty | Spec | Color | Purpose | Cost |
|------|-----|------|-------|---------|------|
| Standard LED | 1 | 5mm, 20mA max | Red | Stop signal | $0.10 |
| Standard LED | 1 | 5mm, 20mA max | Amber/Yellow | Caution signal | $0.10 |
| Standard LED | 1 | 5mm, 20mA max | Green | Go signal | $0.10 |

### Current-Limiting Resistors (for LEDs)

| Item | Qty | Spec | Purpose | Cost |
|------|-----|------|---------|------|
| Resistor | 3 | 220Ω 1/4W | LED protection | $0.05 each |

**Resistor Calculation**:
```
V_LED = 2.0V (typical for red/amber/green)
V_supply = 5V
I_desired = 10-20mA (safe operating current)

R = (V_supply - V_LED) / I_desired
R = (5V - 2V) / 0.015A = 200Ω

Use 220Ω standard resistor (next E12 value)
Actual current: (5V - 2V) / 220Ω = 13.6mA ✓
```

### Input Control

| Item | Qty | Spec | Purpose | Cost |
|------|-----|------|---------|------|
| Push Button | 1 | Momentary NO switch | Trigger stop sequence | $0.50 |
| Resistor | 1 | 10kΩ 1/4W (optional) | Pull-down resistor | $0.05 |

**Note**: Arduino Uno has INPUT_PULLUP enabled, so external pull-down is optional

### Audio Output

| Item | Qty | Spec | Frequency | Purpose | Cost |
|------|-----|------|-----------|---------|------|
| Buzzer | 1 | 2-5kHz active buzzer | 2000Hz | Audible alert | $1-3 |

**OR**

| Item | Qty | Spec | Frequency | Purpose | Cost |
|------|-----|------|-----------|---------|------|
| Speaker | 1 | 8Ω, 0.5W | Variable | Audible alert | $2-5 |

### Prototyping

| Item | Qty | Spec | Purpose | Cost |
|------|-----|------|---------|------|
| Breadboard | 1 | 830 tie-points | Circuit assembly | $3-5 |
| Jumper Wires | ~30 | 22AWG, various colors | Connections | $3-5 |

### Power Supply

| Item | Qty | Spec | Purpose | Cost |
|------|-----|------|---------|------|
| USB Power | 1 | 5V 500mA | Via Arduino USB | $0 (included) |

**Optional for Higher Current**:
| Item | Qty | Spec | Purpose | Cost |
|------|-----|------|---------|------|
| External 5V Supply | 1 | 1A regulated PSU | Backup power | $5-10 |

---

## 💰 Total BOM Cost Estimate

| Category | Cost |
|----------|------|
| Microcontroller | $23-30 |
| LEDs & Resistors | $1-2 |
| Button | $0.50 |
| Buzzer | $1-3 |
| Prototyping Materials | $6-10 |
| **Total (USD)** | **$32-45** |

---

## 🔌 Pin Configuration Reference

### Pin Definitions (src/main.cpp)

```cpp
#define amb_led 3       // Amber LED output
#define grn_led 4       // Green LED output
#define red_led 5       // Red LED output
#define push_button 2   // Push button input (interrupt-capable)
#define buzzer 6        // Buzzer output (tone generation)
// LED_BUILTIN         // Digital 13 - Status indicator (optional)
```

### Arduino UNO Pin Layout

| Pin | Type | Function | Component | Notes |
|-----|------|----------|-----------|-------|
| D2 | INPUT | Push Button | Button | Interrupt-capable (INT0) |
| D3 | OUTPUT | Amber LED | LED + 220Ω resistor | PWM capable |
| D4 | OUTPUT | Green LED | LED + 220Ω resistor | PWM capable |
| D5 | OUTPUT | Red LED | LED + 220Ω resistor | PWM capable |
| D6 | OUTPUT | Buzzer | Active buzzer/speaker | Tone generation |
| D13 | OUTPUT | Built-in LED | Status indicator | Optional |
| GND | GND | Ground | Common ground | Must connect all components |
| 5V | PWR | Power | All components | 5V regulated output |

---

## 🎨 Circuit Diagrams

### Complete Wiring Diagram (Breadboard Layout)

```
Breadboard View:

                    USB Power
                       |
                    5V  |  GND
                    |   |   |
    ┌───────────────┴───┴───┴────────────────┐
    |                                        |
    | Arduino UNO                     Power  |
    |     D2 ──→ Push Button          Rails  |
    |     D3 ──→ Amber LED (Orange)          |
    |     D4 ──→ Green LED                   |
    |     D5 ──→ Red LED                     |
    |     D6 ──→ Buzzer                      |
    |    GND ──→ Common Return               |
    |                                        |
    └────────────────────────────────────────┘
```

### Detailed Connection Diagram

```
5V Rail (Red)
  ├─→ Push Button (Left pin)
  ├─→ Buzzer (+ / Red wire)
  └─→ Power Rail

GND Rail (Black)
  ├─→ Push Button (Right pin via GND)
  ├─→ LED Cathodes (-) ──→ 3 LEDs
  ├─→ Buzzer (- / Black wire)
  └─→ Arduino GND

D2 (White wire)
  ├─→ Push Button (Right pin with pull-up to 5V)
  └─→ Interrupt input (FALLING edge detection)

D3 (Orange wire)
  ├─→ Amber LED Anode via 220Ω resistor
  └─→ Amber LED Cathode to GND

D4 (Green wire)
  ├─→ Green LED Anode via 220Ω resistor
  └─→ Green LED Cathode to GND

D5 (Red wire)
  ├─→ Red LED Anode via 220Ω resistor
  └─→ Red LED Cathode to GND

D6 (Yellow/Blue wire)
  └─→ Buzzer Signal (positive terminal)
```

### Schematic (ASCII Art)

```
     ╔═══════════════════╗
     ║  Arduino Uno      ║
     ║                   ║
    5V ╫──────┬───────┬──┫ D2 ─→ Push Button (INPUT_PULLUP)
     │  │   │   │  │
     │  │   R   │  │  Internal pull-up
     │  │  10k  │  │
     │  └───────┘  │
    GND ╫──────────┴──┫ D3 ─→ 220Ω ─→ Amber LED ──→ GND
     │
    5V ╫──────────────┫ D4 ─→ 220Ω ─→ Green LED ──→ GND
     │
    5V ╫──────────────┫ D5 ─→ 220Ω ─→ Red LED ────→ GND
     │
    5V ╫──────────────┫ D6 ─→ Buzzer ───────────→ GND
     │
    GND╫──────────────┴──┫ GND (Common return)
     └────────────────────┘
```

---

## 🔗 Detailed Wiring Instructions

### Push Button Connection (Interrupt Input)

```cpp
// Pin Configuration
#define push_button 2   // D2 (INT0 interrupt pin)

// Arduino Pull-up Configuration
pinMode(push_button, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(push_button), stopTraffic, FALLING);
```

**Wiring**:
- Button Pin 1 (Left) → Arduino 5V rail
- Button Pin 2 (Right) → Arduino D2 (INPUT_PULLUP)
- Button Pin 2 (Right) → GND rail (via button connection)

**Why INPUT_PULLUP?**
- Arduino has internal 20kΩ pull-up resistor on D2
- When button pressed: D2 = LOW (active)
- When button released: D2 = HIGH (inactive)
- FALLING edge triggers interrupt

### LED Connections (Traffic Lights)

#### Red LED (D5)

```
Arduino D5 ─→ 220Ω Resistor ─→ Red LED Anode (+)
                                Red LED Cathode (-) ─→ GND
```

**Specifications**:
- LED Forward Voltage: 2.0V @ 10-20mA
- Resistor Current Limiting: (5V - 2V) / 220Ω = 13.6mA ✓
- Power Dissipation: 13.6mA × 3V = 40.8mW

#### Amber/Yellow LED (D3)

```
Arduino D3 ─→ 220Ω Resistor ─→ Amber LED Anode (+)
                                 Amber LED Cathode (-) ─→ GND
```

**Specifications**:
- LED Forward Voltage: 2.0V @ 10-20mA
- Resistor Current Limiting: Same as red
- Power Dissipation: ~40mW

#### Green LED (D4)

```
Arduino D4 ─→ 220Ω Resistor ─→ Green LED Anode (+)
                                 Green LED Cathode (-) ─→ GND
```

**Specifications**:
- LED Forward Voltage: 2.0V @ 10-20mA
- Resistor Current Limiting: Same as red
- Power Dissipation: ~40mW

**Total LED Power**: ~120mW (well within safe limits)

### Buzzer Connection (Audio Output)

#### Active Buzzer (Recommended)

```
Arduino D6 ─→ 1kΩ Resistor (optional) ─→ Buzzer (+)
              Buzzer (-) ─→ GND
```

**Specifications**:
- Frequency: 2000 Hz (fixed internal oscillator)
- Voltage: 5V DC
- Current: ~20-30mA
- Code: `tone(buzzer, 2000); // 2000 Hz`
- Stop: `noTone(buzzer);`

**Advantages**:
- Simple control (digital pin)
- Built-in oscillator (no PWM needed)
- Consistent frequency
- Lower power consumption

#### Speaker (Alternative)

```
Arduino D6 ─→ 100Ω Resistor ─→ Speaker (+)
              Speaker (-) ─→ GND
```

**Specifications**:
- Impedance: 8Ω typical
- Power: 0.5W max
- Frequency: Variable via PWM
- Code: `tone(buzzer, 2000);` (uses PWM)

**Advantages**:
- Variable frequency capability
- Better sound quality
- Can play melodies

### Status LED Connection (Built-in LED - Optional)

```
Arduino D13 ─→ Built-in LED (with resistor on board)
               └─→ GND (via resistor)
```

**Current Code Usage**:
```cpp
digitalWrite(LED_BUILTIN, LOW);   // Indicates normal operation (green)
digitalWrite(LED_BUILTIN, HIGH);  // Indicates stop sequence (red)
```

---

## 🔌 Complete Connection Checklist

### Power Connections
- [ ] USB cable connected to Arduino and computer/power adapter
- [ ] 5V rail has stable 5V output (verify with multimeter)
- [ ] GND rail connected to USB ground
- [ ] All components share common GND

### LED Connections
- [ ] Red LED anode → D5 via 220Ω resistor
- [ ] Amber LED anode → D3 via 220Ω resistor
- [ ] Green LED anode → D4 via 220Ω resistor
- [ ] All LED cathodes → GND rail
- [ ] All 220Ω resistors properly rated
- [ ] No LED pins shorted together

### Button Connection
- [ ] Push button left pin → 5V rail
- [ ] Push button right pin → D2 (with pull-up)
- [ ] Push button right pin → GND (through button contact)
- [ ] Button works when pressed (LED should react)

### Buzzer Connection
- [ ] Buzzer + (red) → D6 (via 1kΩ resistor, optional)
- [ ] Buzzer - (black) → GND rail
- [ ] Buzzer makes sound during stop sequence
- [ ] Check volume is appropriate

### Ground Connections
- [ ] Arduino GND → Breadboard GND rail (multiple points)
- [ ] All component grounds connected to same GND rail
- [ ] No floating ground points
- [ ] Continuity between all GND connections verified

---

## 📊 Electrical Specifications

### Current Draw Analysis

| Component | Voltage | Current | Status |
|-----------|---------|---------|--------|
| Arduino UNO | 5V | ~50mA | Idle state |
| Red LED (on) | 2.0V | 13.6mA | Active |
| Amber LED (on) | 2.0V | 13.6mA | Active |
| Green LED (on) | 2.0V | 13.6mA | Active |
| Buzzer (on) | 5V | 25mA | Active |
| Button | 5V | ~1µA | Negligible |
| **Total (all on)** | - | **~115mA** | Peak |
| **Typical Operation** | - | **~80mA** | Average |

### USB Power Limit

- USB 2.0: 500mA maximum available
- USB 3.0: 900mA maximum available
- Project draws: ~115mA maximum
- **Headroom**: ~385mA (USB 2.0) ✓ Safe

### Power Consumption by Mode

**Normal Mode** (Green LED only):
- Arduino: 50mA
- Green LED: 13.6mA
- Total: ~63mA

**Stop Sequence** (All lights + buzzer):
- Arduino: 50mA
- All 3 LEDs: 40.8mA
- Buzzer: 25mA
- Total: ~115mA (for ~12 seconds)

---

## 🔧 Component Specifications

### Arduino Uno R3

| Parameter | Value | Notes |
|-----------|-------|-------|
| Microcontroller | ATmega328P | 8-bit AVR |
| Operating Voltage | 5V | Regulated |
| Input Voltage | 7-12V | Via power jack (when not USB) |
| Digital I/O | 14 pins | 6 PWM capable |
| Analog Inputs | 6 pins | 10-bit ADC |
| Flash Memory | 32 KB | Program storage |
| SRAM | 2 KB | Working memory |
| EEPROM | 1 KB | Persistent storage |
| Clock Speed | 16 MHz | Crystal oscillator |
| Interrupt Pins | D2, D3 | INT0, INT1 |

### LEDs (5mm, Standard)

| Parameter | Red | Amber | Green |
|-----------|-----|-------|-------|
| Wavelength | 625nm | 590nm | 525nm |
| Forward Voltage | 2.0V | 2.0V | 2.0V |
| Max Current | 30mA | 30mA | 30mA |
| Recommended Current | 10-20mA | 10-20mA | 10-20mA |
| Power Dissipation | 40mW | 40mW | 40mW |
| Brightness | High | High | High |
| Viewing Angle | 20° typical | 20° typical | 20° typical |

### Current-Limiting Resistors

| Parameter | Value |
|-----------|-------|
| Resistance | 220Ω |
| Power Rating | 1/4W minimum |
| Tolerance | ±5% (E series) |
| Operating Temperature | -55°C to +125°C |
| Color Code | Red-Red-Brown (220Ω) |

### Push Button

| Parameter | Value |
|-----------|-------|
| Type | Momentary contact, NO (normally open) |
| Contact Resistance | <100mΩ |
| Max Current | 50mA DC |
| Max Voltage | 125V DC |
| Bounce Time | 10-50ms (typical) |
| Mechanical Life | 100,000+ cycles |
| Operating Force | ~50-100 grams |

### Active Buzzer

| Parameter | Value |
|-----------|-------|
| Operating Voltage | 4-8V DC (5V typical) |
| Operating Frequency | ~2kHz (fixed) |
| Sound Output | ~85dB @ 10cm |
| Current Draw | 20-30mA |
| Operating Temperature | -20°C to +70°C |
| Dimensions | 12mm × 9.2mm |
| Polarity | Red (+), Black (-) |

---

## 🛠️ Assembly Instructions

### Step 1: Prepare Breadboard

1. Place breadboard on flat surface
2. Identify power rails (typically top and bottom)
3. Mark 5V (red) and GND (black) rails with tape
4. Ensure all holes are accessible

### Step 2: Power Distribution

1. Connect USB cable to Arduino
2. Place Arduino on left side of breadboard
3. Connect Arduino 5V pin → 5V (red) rail
4. Connect Arduino GND pin → GND (black) rail
5. Verify voltage on breadboard with multimeter (should read ~5V between red and black rails)

### Step 3: Push Button Connection

1. Place push button on breadboard
2. Connect left pin → 5V rail
3. Connect right pin → Arduino D2 pin
4. Press button to test
5. Verify Arduino detects button press (check serial output)

### Step 4: LED Assembly

**For each LED (Red, Amber, Green)**:

1. Identify LED polarity (long lead = anode/+, short lead = cathode/-)
2. Connect resistor (220Ω) to LED anode (long lead)
3. Connect other end of resistor to Arduino pin (D5 for red, D3 for amber, D4 for green)
4. Connect LED cathode (short lead) to GND rail
5. Test LED by uploading code and verifying it lights up

### Step 5: Buzzer Connection

1. Identify buzzer polarity (red = +, black = -)
2. Connect buzzer (+) to Arduino D6 pin (via optional 1kΩ resistor)
3. Connect buzzer (-) to GND rail
4. Test buzzer during stop sequence
5. Adjust volume if using active buzzer with volume potentiometer

### Step 6: Verification

1. Upload code to Arduino
2. Open serial monitor at 9600 baud
3. Verify initialization messages appear
4. Test each component:
   - Green LED should light continuously
   - Press button and observe 3-second delay
   - Verify stop sequence executes
   - Check buzzer sound
   - Verify amber and red lights work

---

## 🐛 Troubleshooting Hardware

### LED Issues

**Problem**: LED doesn't light up
- **Check Polarity**: Long lead (anode) must connect to resistor/pin, short lead to GND
- **Check Resistor**: Verify 220Ω resistor is in correct position
- **Check Pin**: Verify Arduino pin is correct (D3, D4, D5)
- **Test with Multimeter**: Measure voltage at LED anode (should be ~3-5V when on)
- **Bad LED**: Try replacing with known working LED

**Problem**: LED too dim
- **Resistor Too High**: Try 150Ω or 100Ω (consult LED spec)
- **Loose Connection**: Check breadboard contacts, reseat wires
- **Weak Power**: Verify 5V is stable
- **Old LED**: May have degraded brightness

**Problem**: LED too bright (uncomfortable)
- **Increase Resistor**: Try 330Ω or 470Ω
- **Limit Current**: Resistor should limit to 10-15mA
- **Use Diffuser**: Add frosted cover if available

**Problem**: LED flickers
- **Loose Breadboard Contact**: Press wires firmly into breadboard
- **Intermittent Connection**: Check for corrosion on breadboard contacts
- **Power Supply Issue**: Verify USB power is stable
- **Loose Resistor**: Ensure resistor leads are fully inserted

### Button Issues

**Problem**: Button doesn't respond
- **Pin Configuration**: Verify INPUT_PULLUP is set in code
- **Pin Number**: Confirm button is on D2 (INT0)
- **Connection**: Check button pins are connected correctly
- **Bounce**: Button bouncing may prevent detection (usually handled by ISR)
- **Bad Button**: Try replacing push button

**Problem**: Button triggers constantly
- **Floating Pin**: Button must be connected to both 5V and D2
- **Loose Connection**: Ensure button is fully seated
- **Code Issue**: Check ISR logic in `stopTraffic()` function

**Problem**: Button slow to respond
- **Debounce Delay**: Normal 10-50ms bounce is handled by hardware
- **Code Blocking**: Waiting in main loop might delay detection
- **Serial Output**: Excessive Serial.println() calls slow down loop

### Buzzer Issues

**Problem**: Buzzer doesn't make sound
- **Connection**: Verify buzzer is connected to D6
- **Polarity**: Red (+) to D6, Black (-) to GND
- **Volume**: Check if buzzer has volume control (turn up)
- **Test Code**: Verify `tone(buzzer, 2000)` is called
- **Bad Buzzer**: Try replacing with known working buzzer
- **Wokwi Only**: Simulator requires audio enabled in browser settings

**Problem**: Buzzer very quiet
- **Volume Control**: If available, adjust on buzzer module
- **Wokwi Simulator**: Enable audio in browser settings
- **Speaker Alternative**: Try passive speaker instead of active buzzer
- **Frequency**: 2000Hz should be audible; verify in oscilloscope if available

**Problem**: Buzzer makes wrong frequency
- **Check Code**: Verify `tone(buzzer, 2000)` uses correct frequency
- **Active Buzzer**: Has fixed frequency (can't change)
- **Passive Speaker**: Should work with different frequencies
- **Alternative**: Use speaker and PWM for more control

### Power Issues

**Problem**: Arduino resets during stop sequence
- **Insufficient Power**: All LEDs + buzzer draw ~115mA
- **USB Power Limit**: Try external power supply
- **Loose USB**: Ensure USB cable is firmly connected
- **Voltage Drop**: Check 5V with multimeter (should stay above 4.5V)

**Problem**: LEDs dim when buzzer is on
- **Power Supply**: Combined draw causes voltage sag
- **USB Cable**: Try different cable with better connectors
- **External Power**: Use 5V regulated supply instead of USB

**Problem**: Breadboard not supplying power
- **Power Rails**: Verify 5V and GND are continuous
- **Connections**: Check Arduino to breadboard connections
- **Bad Contact**: Breadboard contacts may need cleaning
- **Multimeter Test**: Measure voltage between 5V and GND rails

### General Troubleshooting

**Problem**: Serial output shows garbage characters
- **Baud Rate**: Verify serial monitor is set to 9600 baud
- **Wrong COM Port**: Select correct Arduino COM port
- **Wrong Board**: Verify Arduino UNO selected in IDE
- **Loose USB**: Try different USB port or cable

**Problem**: Code won't upload
- **Board Selection**: Verify Arduino UNO selected
- **COM Port**: Ensure correct port is selected
- **Driver**: May need CH340 driver (clone boards)
- **Cable**: Try different USB cable
- **Reset**: Press reset button during upload

**Problem**: Intermittent behavior
- **Loose Connections**: Check all breadboard wires
- **Cold Solder Joints**: If soldered, reheat joints
- **EMI/Noise**: Keep away from high-frequency sources
- **Power Quality**: Use quality USB power adapter

---

## 🔍 Testing & Validation

### Visual Inspection Checklist

- [ ] All wires are firmly seated in breadboard
- [ ] No wires crossing over each other (causes shorts)
- [ ] LED polarity is correct (long lead to resistor)
- [ ] Button pins are correctly positioned
- [ ] Resistors have color bands matching specification
- [ ] No exposed wire ends touching other components
- [ ] Power rails are not shorted together (5V and GND separated)

### Electrical Measurements

| Measurement | Expected | Actual | Pass/Fail |
|-------------|----------|--------|-----------|
| 5V Rail Voltage | 4.9-5.1V | ___ | _____ |
| GND Continuity | <1Ω | ___ | _____ |
| Green LED ON (D4) | ~2V | ___ | _____ |
| Amber LED ON (D3) | ~2V | ___ | _____ |
| Red LED ON (D5) | ~2V | ___ | _____ |
| Button at D2 | 0V (pressed) / 5V (released) | ___ | _____ |

### Functional Tests

- [ ] **Green LED**: Lights continuously on startup
- [ ] **Button Press**: Triggers 3-second delay (Serial output shows message)
- [ ] **Stop Sequence**: All steps execute in correct order
  - [ ] Green LED turns off
  - [ ] Amber LED flashes 5 times
  - [ ] Red LED turns on
  - [ ] Buzzer beeps (20 times)
  - [ ] Amber LED flashes 3 more times
- [ ] **Reset**: After sequence completes, returns to green
- [ ] **Serial Output**: All debug messages appear at 9600 baud
- [ ] **Total Duration**: Stop sequence takes ~12 seconds (measured)

---

## 📋 Component Alternatives

### LED Alternatives

| Type | Brightness | Cost | Notes |
|------|-----------|------|-------|
| Standard 5mm | ~100mcd | $0.10 | Good for prototyping |
| High-Brightness 5mm | ~500mcd | $0.20 | Better visibility |
| RGB LED | Adjustable | $0.50 | Can mix colors |
| 10mm LED | Brighter | $0.15 | Larger size |
| SMD LED | Compact | $0.05 | Surface mount only |

### Buzzer Alternatives

| Type | Frequency | Cost | Notes |
|------|-----------|------|-------|
| Active Buzzer | Fixed 2kHz | $1-2 | Simplest, recommended |
| Passive Speaker | Variable | $2-5 | More flexible, needs PWM |
| Piezo Transducer | Variable | $0.50 | Compact, lower volume |
| Speaker Module | Variable | $3-8 | Professional sound quality |

### Resistor Alternatives

| Value | Current (5V, 2V LED) | Cost | Notes |
|-------|------------------|------|-------|
| 150Ω | 20mA | $0.05 | Brighter, hotter |
| 220Ω | 13.6mA | $0.05 | **Recommended** |
| 330Ω | 9.1mA | $0.05 | Dimmer, cooler |
| 470Ω | 6.8mA | $0.05 | Very dim |

---

## 📸 Wokwi Simulation

### Wokwi Diagram Components

The `Wokwi/diagram.json` includes:

```json
{
  "parts": [
    { "type": "wokwi-arduino-uno", "id": "uno" },           // Microcontroller
    { "type": "wokwi-led", "color": "red" },                // Red LED
    { "type": "wokwi-led", "color": "orange" },             // Amber LED
    { "type": "wokwi-led", "color": "green" },              // Green LED
    { "type": "wokwi-resistor", "value": "220" },           // 220Ω resistor x3
    { "type": "wokwi-pushbutton" },                         // Push button
    { "type": "wokwi-buzzer" },                             // Buzzer
    { "type": "wokwi-logic-analyzer" }                      // Debug analyzer
  ]
}
```

### Simulation Setup

1. **Build Firmware**:
   ```bash
   pio run -e uno
   ```

2. **Open in Wokwi**:
   - Go to https://wokwi.com
   - Create new project or load from `Wokwi/diagram.json`

3. **Configure Firmware Path** (in `Wokwi/wokwi.toml`):
   ```toml
   firmware = '../.pio/build/uno/firmware.hex'
   elf = '../.pio/build/uno/firmware.elf'
   ```

4. **Run Simulation**:
   - Click "Play" button in Wokwi
   - Press simulated button to trigger stop sequence
   - Observe LED changes and buzzer activity

---

## 🎯 Design Considerations

### Layout Best Practices

1. **Component Placement**:
   - Group LEDs together on one side
   - Place resistors near LED anodes
   - Position button in center for easy access
   - Place buzzer away from audio-sensitive circuits

2. **Wire Routing**:
   - Keep signal wires (D pins) separate from power wires
   - Bundle 5V and GND wires together for symmetry
   - Minimize wire lengths (shorter = less noise)
   - Use color-coded wires (red=5V, black=GND, others=signals)

3. **Thermal Considerations**:
   - LEDs generate minimal heat (~40mW each)
   - Resistors generate ~40mW (negligible)
   - No heatsinks required
   - Breadboard can handle ~1W continuous power

4. **Noise Reduction**:
   - Keep digital lines away from analog circuits
   - Use short, direct connections
   - Ground plane would improve EMI immunity (not needed for this project)

### Safety Considerations

- **Voltage**: 5V is safe (no shock hazard)
- **Current**: <200mA total (safe for USB power)
- **Heat**: Minimal heat generation (all components stay cool)
- **Short Circuit**: 220Ω resistors protect against LED shorts
- **USB Overpowering**: USB current limiter protects against total draws >500mA

---

## 📚 Additional Resources

### Component Datasheets
- [Arduino UNO Datasheet](https://store.arduino.cc/products/arduino-uno-rev3)
- [ATmega328P Spec](https://ww1.microchip.com/datasheet/ATmega328P)
- [Common LED Specifications](https://www.electronics-tutorials.ws/diode/diode_3.html)

### Simulation Tools
- [Wokwi.com](https://wokwi.com) - Arduino simulator
- [Tinkercad Circuits](https://www.tinkercad.com/circuits) - Circuit simulator
- [LTspice](https://www.analog.com/en/design-center/design-tools-and-calculators/ltspice-simulator.html) - SPICE simulator

### Build Tips
- [Breadboard Guide](https://www.electronics-tutorials.ws/blog/breadboard.html)
- [Resistor Color Codes](https://en.wikipedia.org/wiki/Electronic_color_code)
- [LED Calculations](https://www.instructables.com/LEDs-for-Beginners/)

---

## 🔗 Related Documentation

- **[README.md](../README.md)** - Project overview
- **[API.md](../API.md)** - Software API reference
- **[CONTRIBUTING.md](../CONTRIBUTING.md)** - Development guidelines
- **[CHANGELOG.md](../CHANGELOG.md)** - Version history

---

**Last Updated**: 2026-03-04
**Hardware Revision**: 1.0
**Compatibility**: Arduino UNO R3
**Status**: Tested & Verified ✓