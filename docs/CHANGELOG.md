# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Planned

- [ ] Debounce logic for button (current: edge-triggered only)
- [ ] Configurable timing parameters (transition delay, flash intervals)
- [ ] Multiple traffic light scenarios (standard, pedestrian crossing, emergency)
- [ ] Unit tests for timing calculations

## [1.0.0] - 2026-03-04

### Added

- Initial traffic light controller using Arduino Uno
- Hardware interrupt-driven button handling (pin D2, FALLING edge)
- Three-color LED system (Red/Amber/Green on pins D5/D3/D4)
- Audible buzzer alert (pin D6, 2000Hz tone)
- Built-in LED status indicator (ON=normal, OFF=stop mode)
- Complete stop sequence: amber flash → red ON → buzzer beeps → amber flash
- 3-second transition delay after button press
- Serial debug logging at 9600 baud
- PlatformIO build configuration for Arduino Uno
- Wokwi simulation setup with firmware linking
- Comprehensive work log (WORKLOG.md)

### Technical Details

- ISR: `stopTraffic()` on pin D2 (active-low, INPUT_PULLUP)
- State management: `stopRequested` and `stopSequenceRunning` flags
- Timing: 5 amber flashes (500ms each) before red, 3 after, 20 buzzer beeps
- Total stop sequence duration: ~10-12 seconds (measured at runtime)
- GPIO: 6 outputs (3x LED, 1x buzzer, 1x built-in LED)

### Fixed

- Buzzer audibility in Wokwi simulator (increased volume in diagram.json)
- Replaced basic digitalWrite buzzer control with `tone()`/`noTone()` for better frequency control

### Changed

- Improved buzzer driving method for cleaner audio output

### Known Issues

- Serial output every 250ms during normal state may cause performance lag
- No debounce logic (relies on ISR edge detection)
- Button presses during stop sequence are ignored (by design - safety feature)
- Buzzer frequency fixed at 2000Hz (no user control)
- Redundant `stopRequested = true` assignment in stopTraffic() function

---

## Version History Summary

| Version | Date | Major Changes |
| ------- | ---- | ------------- |
| 1.0.0 | 2026-03-04 | Initial release with full traffic light functionality |

## Migration Guide

### For new users

- Start with the README for hardware setup
- Review WORKLOG.md for recent fixes (especially buzzer setup)
- Test in Wokwi before deploying to real hardware

### For contributors

- Review API.md for function signatures
- Check CONTRIBUTING.md before submitting PRs
- Run full test suite (see Contributing for checklist)
