# Contributing to Simple Traffic Light

Thank you for your interest in contributing to the Simple Traffic Light project! We welcome contributions from developers of all skill levels. This document provides comprehensive guidelines for contributing code, documentation, and improvements.

---

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [Getting Started](#getting-started)
3. [Development Setup](#development-setup)
4. [Making Changes](#making-changes)
5. [Code Guidelines](#code-guidelines)
6. [Testing](#testing)
7. [Submitting Changes](#submitting-changes)
8. [Pull Request Process](#pull-request-process)
9. [Common Issues](#common-issues)
10. [Recognition](#recognition)

---

## Code of Conduct

### Our Commitment

We are committed to providing a welcoming and inclusive environment for all contributors. We expect all participants to:

- **Be Respectful**: Treat all contributors with courtesy and respect
- **Be Constructive**: Offer helpful feedback and suggestions
- **Be Inclusive**: Welcome people of all backgrounds and experience levels
- **Be Professional**: Keep discussions focused on the project

### Unacceptable Behavior

The following behaviors are not tolerated:

- Harassment, discrimination, or personal attacks
- Offensive or derogatory language
- Trolling or intentionally disruptive comments
- Publishing others' private information without consent
- Any form of abuse or intimidation

### Reporting Issues

If you experience or witness unacceptable behavior, please report it by:

- Emailing the maintainers privately
- Opening a confidential GitHub issue
- Using the GitHub abuse reporting feature

---

## Getting Started

### Before You Start

1. **Familiarize Yourself** with the project:
   - Read [README.md](../README.md) for project overview
   - Review [API.md](API.md) for technical details
   - Check [HARDWARE.md](HARDWARE.md) for hardware specifications
   - Examine [CHANGELOG.md](CHANGELOG.md) for project history

2. **Check Existing Issues**:
   - Search for related issues before creating new ones
   - Look at open pull requests to avoid duplicate work
   - Review closed issues to understand past decisions

3. **Start Small**:
   - First contribution? Look for issues labeled `good first issue`
   - Medium difficulty? Try issues labeled `help wanted`
   - Want to learn? Issues labeled `documentation` are great for learning the codebase

4. **Ask Questions**:
   - Unsure about something? Open a discussion or issue
   - Don't hesitate to ask for clarification
   - Community is here to help!

### Types of Contributions

We welcome contributions in many forms:

| Type | Examples | Difficulty |
| --- | --- | --- |
| **Bug Fixes** | Fix incorrect behavior, memory leaks | Medium |
| **Features** | New LED modes, configurable timings, debouncing | Medium-Hard |
| **Documentation** | Fix typos, improve clarity, add examples | Easy |
| **Tests** | Unit tests, integration tests | Medium |
| **Optimization** | Reduce code size, improve efficiency | Hard |
| **Refactoring** | Improve code structure, remove duplication | Medium |

---

## Development Setup

### Prerequisites

- **Git**: For version control
- **Python 3.7+**: For PlatformIO
- **PlatformIO CLI**: For building and uploading
- **Arduino UNO** (optional): For hardware testing
- **Text Editor**: VS Code recommended (with PlatformIO extension)

### Step 1: Fork the Repository

```bash
# Navigate to the project on GitHub
# Click "Fork" button in top right
# Clone your fork locally
git clone https://github.com/YOUR_USERNAME/Simple_Traffic_Light.git
cd Simple_Traffic_Light
```

### Step 2: Add Upstream Remote

```bash
# Add original repo as upstream
git remote add upstream https://github.com/PeraDaq/Simple_Traffic_Light.git

# Verify remotes
git remote -v
# origin    https://github.com/YOUR_USERNAME/Simple_Traffic_Light.git
# upstream  https://github.com/PeraDaq/Simple_Traffic_Light.git
```

### Step 3: Install Dependencies

```bash
# Install PlatformIO
pip install platformio

# Verify installation
pio --version

# Navigate to project and install project dependencies
cd Simple_Traffic_Light
pio lib install
```

### Step 4: Verify Setup

```bash
# Build the project
pio run

# Should see:
# ✓ Compiling firmware...
# ✓ Firmware built successfully
# ✓ No errors or warnings
```

### Step 5: Set Up IDE (VS Code - Optional)

```bash
# Install VS Code PlatformIO extension
# 1. Open VS Code
# 2. Go to Extensions (Ctrl+Shift+X)
# 3. Search "PlatformIO"
# 4. Click "Install"

# Open project
code .
```

---

## Making Changes

### Create a Feature Branch

```bash
# Update main branch from upstream
git fetch upstream
git checkout main
git reset --hard upstream/main

# Create feature branch with descriptive name
git checkout -b feature/add-debouncing
# or
git checkout -b fix/button-interrupt-issue
# or
git checkout -b docs/improve-hardware-guide
```

### Branch Naming Conventions

Use prefixes to categorize your work:

| Prefix | Purpose | Examples |
| --- | --- | --- |
| `feature/` | New functionality | `feature/add-pwm-led-control` |
| `fix/` | Bug fixes | `fix/buzzer-frequency-off` |
| `docs/` | Documentation | `docs/add-wokwi-guide` |
| `refactor/` | Code improvements | `refactor/extract-led-control` |
| `test/` | Testing additions | `test/add-interrupt-tests` |
| `perf/` | Performance | `perf/reduce-memory-usage` |

### Branch Rules

- Use **lowercase** with **hyphens** (kebab-case)
- Be **descriptive** and **concise**
- Keep **under 50 characters** if possible
- **Don't use** issue numbers in branch name (link in PR instead)

### Example Workflow

```bash
# Create and switch to feature branch
git checkout -b feature/add-configurable-timing

# Make changes
vim src/main.cpp
vim HARDWARE.md

# Stage changes
git add src/main.cpp HARDWARE.md

# Commit with clear message (see next section)
git commit -m "Add configurable LED timing via #defines

- Add TRANSITION_DELAY, FLASH_INTERVAL macros
- Update documentation with timing constants
- Update HARDWARE.md with calibration guide"

# Keep updated with upstream
git fetch upstream
git rebase upstream/main

# Push to your fork
git push origin feature/add-configurable-timing

# Go to GitHub and open Pull Request
```

---

## Code Guidelines

### C/C++ Code Style

#### Naming Conventions

```cpp
// Constants: UPPER_CASE
#define TRANSITION_DELAY 3000      // Not: transitionDelay or transition_delay
#define BUZZER_FREQUENCY 2000      // Global constants in #define
const int LED_PIN = 4;             // Or const variables

// Variables: snake_case
int stop_sequence_duration = 0;    // Not: stopSequenceDuration
bool is_sequence_running = false;  // For boolean flags, use 'is_' prefix

// Functions: camelCase (lowercase first letter)
void setupLeds();                  // Not: SetupLeds or setup_leds
void runStopSequence();            // Verb first
bool isInterruptReady();            // Boolean functions use 'is_' prefix

// Class members: snake_case with trailing underscore (if used)
class TrafficLight {
  int brightness_;
  bool is_active_;
};
```

#### Code Formatting

```cpp
// 2-space indentation (NO tabs)
if (stopRequested) {
  digitalWrite(grn_led, LOW);      // 2-space indent
  delay(500);
  digitalWrite(amb_led, HIGH);
}

// Line length: Keep under 100 characters
// Bad:
digitalWrite(grn_led, LOW); delay(500); digitalWrite(amb_led, HIGH); digitalWrite(amb_led, LOW);

// Good:
digitalWrite(grn_led, LOW);
delay(500);
digitalWrite(amb_led, HIGH);
digitalWrite(amb_led, LOW);

// Comments: Explain "why", not "what"
// Bad:
digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED

// Good:
digitalWrite(LED_BUILTIN, HIGH);  // Indicate stop mode in progress

// Function comments: Document purpose, parameters, return value
/**
 * Execute the complete traffic light stop sequence.
 * 
 * Behavior:
 * - Green OFF (500ms)
 * - Amber flash 5x (500ms each)
 * - Red ON (stays on)
 * - Buzzer beeps 20x (100ms each)
 * - Amber flash 3x (500ms each)
 * 
 * @return void
 * @note Duration: ~12 seconds. Sets stopSequenceRunning flag.
 */
void runStopSequence() {
  // Implementation...
}

// Spacing: One space after control keywords
if (condition) {          // Not: if(condition)
  // ...
}

for (int i = 0; i < 5; i++) {  // Not: for(int i=0;i<5;i++)
  // ...
}

// Braces: Allman style (opening brace on new line for blocks)
if (condition)
{
  statement1();
  statement2();
}
else
{
  statement3();
}
```

#### Variable Declaration

```cpp
// Keep declarations close to usage
void loop() {
  // Read sensor first
  int adcValue = analogRead(A0);
  
  // Then use immediately
  if (adcValue > 512) {
    digitalWrite(led_pin, HIGH);
  }
}

// Avoid long chains of unrelated declarations
// Bad:
int a, b, c, d, e, f, g;

// Good:
int sensor_value = analogRead(A0);
int threshold = 512;
bool is_active = false;
```

### Arduino-Specific Guidelines

```cpp
// Use Arduino-style function names
void setup() { }        // Not: Setup() or void main()
void loop() { }         // Main loop function

// Use Arduino constants
digitalWrite(pin, HIGH);     // Not: digitalWrite(pin, 1)
pinMode(pin, INPUT_PULLUP);  // Use named constants

// Prefer digitalWrite for digital pins
digitalWrite(led_pin, HIGH);  // Clear intent

// Use appropriate timing
delay(100);              // Milliseconds
millis();               // Current time in milliseconds
micros();               // High-resolution timing

// ISR (Interrupt Service Routine) best practices
volatile bool flag = false;  // Volatile for ISR-shared variables

void isr_handler() {
  // Keep ISR SHORT
  // Avoid Serial, delay, complex logic
  flag = true;  // Just set flag
}
```

### Comments and Documentation

```cpp
// Header comments: Explain module purpose
/**
 * Simple Traffic Light Controller
 * 
 * Implements interrupt-driven traffic light with:
 * - Normal state: Green LED ON
 * - Stop sequence: Amber flash → Red ON → Buzzer
 * - Duration: ~12 seconds for complete sequence
 * 
 * Hardware: Arduino UNO
 * Pins: D2 (button), D3-5 (LEDs), D6 (buzzer)
 */

// Section comments: Group related code
// ===== LED CONTROL FUNCTIONS =====
void setupLeds() { }
void turnOnGreen() { }
void turnOnRed() { }

// Inline comments: Explain complex logic (not obvious code)
// Bad:
i++;  // Increment i

// Good:
// Wait for button press before proceeding to stop sequence
while (!stopRequested) {
  // Keep waiting...
}

// Function documentation: JSDoc-style
/**
 * Brief one-liner description.
 * 
 * Longer description of what this function does,
 * including important behavioral notes.
 * 
 * @param paramName Description of parameter
 * @return Description of return value
 * @note Important notes about usage
 * @example
 *   runStopSequence();  // Execute full sequence
 */
```

---

## Testing

### Build Testing

```bash
# Compile without uploading
pio run

# Should output:
# ✓ Building firmware...
# ✓ No errors
# ✓ No warnings (preferred)
```

### Hardware Testing

```bash
# Upload to Arduino
pio run -t upload

# Monitor serial output
pio device monitor -b 9600

# Expected output:
# Setting up Arduino Green, Amber, Red LED Pins (3, 4, 5)...
# Setting up Arduino Push Button "Input Pullup" pin 2...
# Attaching ISR to Push Button pin, monitoring button status continuously!
# Green Traffic Light On!
```

### Wokwi Simulation Testing

```bash
# Build firmware for simulation
pio run

# In Wokwi.com:
# 1. Open project from Wokwi/diagram.json
# 2. Verify firmware path in wokwi.toml
# 3. Start simulation
# 4. Test each feature:
#    - Green LED lights on startup
#    - Button press triggers 3s delay
#    - All LEDs in stop sequence light correctly
#    - Buzzer makes sound
#    - Returns to green after sequence
```

### Functional Testing Checklist

Before submitting, verify:

- [ ] **Compilation**
  - [ ] No compiler errors
  - [ ] No compiler warnings
  - [ ] Firmware builds successfully

- [ ] **Hardware Functionality** (if you have hardware)
  - [ ] Green LED lights on startup
  - [ ] Button press detected
  - [ ] 3-second transition delay works
  - [ ] Amber LED flashes (5 times before red, 3 after)
  - [ ] Red LED turns on during stop sequence
  - [ ] Buzzer makes sound (20 beeps)
  - [ ] Built-in LED indicates state correctly
  - [ ] System returns to green after sequence

- [ ] **Wokwi Simulation**
  - [ ] Simulation starts without errors
  - [ ] All components visible and connected
  - [ ] Button press works in simulation
  - [ ] LEDs light up as expected
  - [ ] Buzzer plays sound (if volume enabled)

- [ ] **Code Quality**
  - [ ] Code follows style guidelines
  - [ ] Comments explain complex logic
  - [ ] Variable names are descriptive
  - [ ] No dead code or commented-out sections
  - [ ] Functions under 50 lines (if possible)

- [ ] **Documentation**
  - [ ] README.md updated (if user-facing change)
  - [ ] Code comments added where needed
  - [ ] API.md updated (if function signatures change)
  - [ ] HARDWARE.md updated (if pin/wiring changes)
  - [ ] Commit message is clear and descriptive

---

## Submitting Changes

### Commit Messages

Write clear, descriptive commit messages:

```bash
# Format
<type>: <subject>

<body>

<footer>

# Example
feat: Add debounce logic to button interrupt

- Implement 10ms debounce delay
- Prevents false triggers from contact bounce
- Improves reliability on noisy breadboards

Fixes #42
```

### Commit Message Types

```text
feat:     A new feature
fix:      A bug fix
docs:     Documentation only changes
style:    Changes that don't affect code meaning (formatting, missing semicolons, etc)
refactor: Code change that neither fixes a bug nor adds a feature
perf:     Code change that improves performance
test:     Adding or updating test files
chore:    Changes to build process, dependencies, or tooling
```

### Commit Message Rules

1. **First Line**:
   - 50 characters or less
   - Use imperative mood ("Add feature" not "Added feature")
   - Don't end with period

2. **Blank Line**:
   - Separate subject from body

3. **Body**:
   - 72 characters per line
   - Explain "what" and "why", not "how"
   - Use bullet points for multiple changes

4. **Footer**:
   - Reference issues: `Fixes #123`, `Related to #456`
   - Link to PRs if related

### Example Commit Messages

**Good**:

```text
Add configurable timing for LED flashing

- Extract hard-coded delays into #defines
- Allow users to adjust timing without recompiling
- Update HARDWARE.md with calibration guide

Fixes #35
```

**Bad**:

```text
Fixed stuff

- Changed some code
- Made it work better
```

---

## Pull Request Process

### Before Opening a PR

1. **Sync with Upstream**:

   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Verify Tests Pass**:

   ```bash
   pio run        # Should compile
   pio run -t upload  # Should upload (if have hardware)
   ```

3. **Check Code Quality**:
   - No trailing whitespace
   - No tabs (use spaces)
   - Comments are clear
   - Variable names descriptive

### Opening a Pull Request

1. **Push to Your Fork**:

   ```bash
   git push origin feature/your-feature-name
   ```

2. **Create PR on GitHub**:
   - Click "New Pull Request" button
   - Ensure base branch is `main`
   - Ensure your branch is selected

3. **Fill PR Template**:

   ```markdown
   ## Description
   Brief description of what this PR does.

   ## Type of Change
   - [ ] Bug fix (non-breaking change that fixes an issue)
   - [ ] New feature (non-breaking change that adds functionality)
   - [ ] Breaking change (fix or feature that would cause existing functionality to change)
   - [ ] Documentation update

   ## Related Issue
   Fixes #(issue number)
   Related to #(other issue numbers)

   ## Testing
   - [ ] Tested on Arduino Uno
   - [ ] Tested in Wokwi simulation
   - [ ] No compilation warnings
   - [ ] All existing functionality works

   ## Checklist
   - [ ] My code follows the style guidelines
   - [ ] I have performed a self-review of my own code
   - [ ] I have commented my code, particularly in hard-to-understand areas
   - [ ] I have made corresponding changes to the documentation
   - [ ] My changes generate no new warnings
   - [ ] I have tested my changes thoroughly
   ```

### PR Title Format

```text
type: Brief description

# Examples:
feat: Add debouncing to button interrupt
fix: Correct buzzer frequency calculation
docs: Improve installation instructions
refactor: Extract LED control into functions
```

### During Review

- **Respond to Comments**: Address feedback promptly
- **Request Re-review**: After making changes, request another review
- **Be Patient**: Reviews take time, maintainers are volunteers
- **Ask Questions**: If feedback is unclear, ask for clarification
- **Keep Updates**: Keep PR updated with latest main branch

### After Approval

- Maintainer will merge your PR
- Squash commits if multiple small commits
- Delete your feature branch
- Celebrate your contribution! 🎉

---

## Common Issues

### Issue: Merge Conflicts

```bash
# Update your branch
git fetch upstream
git rebase upstream/main

# Resolve conflicts in your editor
# Then continue rebase
git add .
git rebase --continue

# Force push to your fork
git push --force-with-lease origin feature-name
```

### Issue: Accidental Commits

```bash
# Undo last commit (keep changes)
git reset --soft HEAD~1

# Undo last commit (discard changes)
git reset --hard HEAD~1

# Amend last commit
git commit --amend
```

### Issue: Build Fails

```bash
# Clean build
pio run --target clean
pio run

# Check for:
# - Wrong board selected: pio boards | grep uno
# - Missing libraries: pio lib install
# - Syntax errors: Check compiler output
```

### Issue: Code Style Problems

```bash
# Check formatting
pio run  # Compiler warnings about style

# Fix issues
# - Use 2-space indentation (not tabs)
# - Check variable naming (snake_case vs camelCase)
# - Verify brace placement
# - Remove trailing whitespace
```

---

## Areas for Contribution

### High Priority

- [ ] **Debounce Logic** - Prevent false button triggers
- [ ] **Configurable Timing** - Make delays user-adjustable
- [ ] **Button Debouncing** - Better edge case handling
- [ ] **Performance Optimization** - Reduce code size

### Medium Priority

- [ ] **Additional Test Coverage** - More thorough testing
- [ ] **Extended Documentation** - Improve guides and examples
- [ ] **Multiple Buttons** - Support multiple triggers
- [ ] **Pedestrian Crossing Mode** - New operational mode

### Documentation Priority

- [ ] **Video Tutorials** - Step-by-step setup videos
- [ ] **Troubleshooting Guide** - Common problems and solutions
- [ ] **Wokwi Setup Guide** - Detailed simulation instructions
- [ ] **Performance Analysis** - Power consumption, timing specs

---

## Recognition

### Contributors

We recognize all types of contributions:

- **Code Contributions**: Credited via GitHub contributors and release notes
- **Bug Reports**: Credited in issue discussions
- **Documentation**: Credited in file headers
- **Reviews**: Mentioned in PR discussions

### Hall of Fame

Major contributors are recognized in:

- GitHub contributors page (automatic)
- Release notes for significant contributions

---

## Additional Resources

### Project Documentation

- [README.md](../README.md) - Project overview
- [API.md](API.md) - Technical API reference
- [HARDWARE.md](HARDWARE.md) - Hardware specifications
- [CHANGELOG.md](CHANGELOG.md) - Version history

### Arduino Resources

- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Arduino Getting Started](https://www.arduino.cc/en/Guide)
- [Interrupt Documentation](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Development Tools

- [PlatformIO Documentation](https://docs.platformio.org)
- [Wokwi Simulator](https://wokwi.com/docs)
- [Git Basics](https://git-scm.com/book)

### Community

- [Arduino Community Forums](https://forum.arduino.cc/)
- [Arduino Discord](https://discord.gg/arduino)
- [GitHub Discussions](https://github.com/PeraDaq/Simple_Traffic_Light/discussions)

---

## Getting Help

### Questions?

- **Open a Discussion**: Use GitHub Discussions for general questions
- **Check Wiki**: Look for existing answers in documentation
- **Ask in Issues**: Include `[QUESTION]` in issue title
- **Join Discord**: Connect with community members

### Problem with Contribution Process?

- **Email Maintainers**: For private concerns
- **Open an Issue**: For public discussion
- **Read This Guide**: Again! It probably answers your question

---

## Contact

- **Issues**: Report bugs and request features on GitHub
- **Discussions**: Ask questions and discuss ideas
- **Email**: Contact maintainers for sensitive matters

---

**Thank you for contributing to Simple Traffic Light!** 🚀

Your contributions, no matter how small, help make this project better for everyone.

---

**Last Updated**: 2026-03-04
**Version**: 1.0
**Status**: Active & Welcoming Contributors
