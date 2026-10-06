# RF_Sniper-Ver.3

<p align="center">
  <img width="600" alt="RF_Sniper-Ver.3 device" src="https://github.com/user-attachments/assets/781c1911-c39b-4b4c-88e4-c0b5dd19047e" />
</p>

A handheld radio frequency spectrum analyzer with touchscreen interface and real-time frequency analysis capability.

RF_Sniper-Ver.3 is a portable RF measurement instrument designed for field spectrum monitoring, signal detection, and frequency analysis in the VHF/UHF band.

The device combines a custom RF front-end, high-speed ADC sampling, DSP-based signal processing, and a color touchscreen interface into a compact handheld platform suitable for amateur radio, RF engineering, and signal investigation.

## Overview

RF_Sniper-Ver.3 is a continuation and improvement of the original Antuino project, adapted with STM32 processor performance and modern touchscreen interface design.

The instrument provides:

- real-time spectrum visualization
- frequency search and signal detection
- signal strength measurement (RSSI)
- peak detection and hold
- frequency resolution and zoom capability
- touchscreen-based navigation and control

The measurement display is intuitive and optimized for field use, with modes for broadband scanning and detailed frequency inspection.

## Key Features

- **Real-Time Spectrum Analyzer** — live RF spectrum display
- **Frequency Range** — 0.5 MHz to 150 MHz coverage
- **Signal Detection** — automatic peak detection and identification
- **RSSI Measurement** — received signal strength indicator
- **Frequency Zoom** — detailed inspection of selected bands
- **Peak Hold** — signal tracking and peak memory
- **Touch Interface** — full touchscreen control and navigation
- **Color Display** — high-contrast ILI9341 TFT screen
- **Portable Design** — compact battery-powered handheld device

---

## Hardware Platform

RF_Sniper-Ver.3 is built on a high-performance embedded platform optimized for RF measurement.

| Component | Selection |
|---|---|
| Microcontroller | STM32F4 series |
| Display | ILI9341 color TFT display |
| Touch Interface | Capacitive or resistive touch controller |
| ADC | High-speed internal ADC |
| RF Front-End | Optimized for 0.5–150 MHz |
| DSP | ARM CMSIS-DSP for signal processing |
| Storage | EEPROM for settings and history |
| Power | Battery-powered operation |

---

## Spectrum Analyzer Capabilities

### Real-Time Frequency Display

The device scans the RF spectrum in real-time and displays signal strength across the frequency range.

Features include:

- continuous spectrum update
- marker for peak signal
- frequency axis scaling
- dB level indication

### Signal Detection and Peak Hold

Automatic detection of signals above the noise floor with optional peak memory for tracking transient signals.

The display can hold the peak level for inspection even after the signal has passed.

### Frequency Zoom and Detail View

Zoom into specific frequency regions for detailed inspection of complex signal environments.

The zoom function allows precise identification of narrow-band signals and frequency drift measurement.

### RSSI Measurement

Receive Signal Strength Indicator (RSSI) provides quantitative signal level measurement in dB and can be logged for trend analysis.

---

## Software Architecture

RF_Sniper-Ver.3 is developed in C/C++ using the STM32 HAL and ARM CMSIS-DSP libraries.

Major software components include:

- STM32 Hardware Abstraction Layer (HAL)
- ARM CMSIS-DSP for FFT and signal processing
- TFT_eSPI or custom display driver
- custom RF front-end interface
- custom touch input handling
- custom measurement algorithms
- custom frequency analysis and visualization

The firmware is organized into layers:

```text
┌─────────────────────────────┐
│      Touch Interface        │
├─────────────────────────────┤
│   Measurement / Analysis    │
├─────────────────────────────┤
│        DSP / FFT            │
├─────────────────────────────┤
│       ADC / Sampling        │
├─────────────────────────────┤
│      RF Front-End           │
└─────────────────────────────┘
```

---

## Signal Processing Pipeline

The RF front-end converts analog signals to digital samples, which are then processed through DSP and FFT stages for spectrum analysis.

```text
RF Input (0.5–150 MHz)
     │
     ▼
Analog Front-End
     │
     ▼
ADC Sampling
     │
     ▼
Digital Buffer
     │
     ▼
DSP Processing
     ├── FFT Analysis
     ├── Peak Detection
     ├── Level Measurement
     └── Frequency Calculation
     │
     ▼
Display Rendering
```

---

## Use Cases

RF_Sniper-Ver.3 is useful for:

- **Amateur Radio** — band monitoring and signal hunting
- **RF Engineering** — field measurements and spectrum survey
- **Signal Investigation** — identifying unknown transmissions
- **Frequency Planning** — detecting interference and occupancy
- **Educational Use** — learning about RF and spectrum analysis
- **Portable Measurements** — handheld field operations

---

## Operating Modes

### Scan Mode

Continuous sweep across the frequency range with real-time peak detection.

### Zoom Mode

Detailed inspection of a selected frequency band with finer resolution.

### Peak Hold Mode

Single measurement with peak signal hold for transient detection.

### History Mode

Review of previously recorded measurements and signal logs.

---

## Development History

RF_Sniper-Ver.3 evolved from the original Antuino project by EB7ME.

Major improvements include:

- STM32F4 processor upgrade for faster processing
- ILI9341 color display implementation
- touchscreen interface development
- improved RF front-end design
- enhanced DSP algorithms
- better measurement accuracy
- refined user interface

The project combines the original concept with modern embedded system design practices.

---

## Project Status

RF_Sniper-Ver.3 is a functional portable RF measurement instrument and is actively under development.

It is primarily intended for:

- amateur radio operation
- RF engineering and measurement
- spectrum monitoring
- field signal analysis
- educational RF exploration

---

## Roadmap

| Feature | Status |
|---|---|
| Real-time spectrum display | Functional |
| Frequency sweep and scan | Functional |
| Peak detection | Functional |
| RSSI measurement | Functional |
| Touch interface | Functional |
| Zoom and detail view | Functional |
| Peak hold | Functional |
| Frequency markers | Planned |
| Signal logging | Planned |
| Measurement history | In development |
| Calibration procedures | Planned |
| Advanced filtering | Planned |

---

## References

**Original Project:**
- EB7ME Antuino — https://github.com/EB7ME/Antuino

**Additional Information:**
- YO6PIR Project Details — https://qsl.net/yo6pir/snipper3.html

---

## Project Principles

- field-proven measurements
- intuitive touchscreen interface
- reliable and stable operation
- battery-powered portability
- easy firmware updates
- responsive peak detection

---

## Building and Flashing

Firmware compilation uses the Arduino IDE or PlatformIO with STM32 board support.

```bash
# Using Arduino IDE:
# 1. Install STM32 board package
# 2. Select board: Generic STM32F4 series
# 3. Configure upload method
# 4. Compile and upload

# Using PlatformIO:
platformio run -t upload
```

Detailed build instructions are available in the project documentation.

---

## License

This project is provided for educational, experimental, and personal development use.

See the repository license for the applicable terms.

---

## Project Credits

**Original Concept:** Andreas EB7ME — Antuino project
**Development & Improvements:** Ovidiu YO6PIR — RF_Sniper-Ver.3

RF_Sniper-Ver.3 represents a practical evolution of portable RF measurement, bringing modern touchscreen interface and STM32 performance to field spectrum analysis.
