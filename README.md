# RF_Sniper-Ver.3

<p align="center">
  <img width="600" alt="RF_Sniper-Ver.3 portable RF analyzer" src="https://github.com/user-attachments/assets/781c1911-c39b-4b4c-88e4-c0b5dd19047e" />
</p>

A handheld RF measurement and tuning instrument for the 0.5–150 MHz band with touchscreen interface and graphical sweep display.

RF_Sniper-Ver.3 is a practical field instrument designed for frequency scanning, signal level measurement, SWR evaluation, and grid-dip operation. Built around an STM32 microcontroller, Si5351 frequency synthesizer, and color TFT display, it provides intuitive touch-based control for RF exploration and antenna tuning work.

---

## Overview

RF_Sniper-Ver.3 combines the original Antuino concept by EB7ME with modern embedded systems design, delivering a compact, battery-powered RF analyzer suitable for amateur radio, signal investigation, and RF experimentation.

The instrument displays real-time sweeps across user-defined frequency ranges and allows direct manipulation of measurement parameters through a responsive touchscreen interface.

Key design goals:

- **Portable operation** — battery-powered field use
- **Practical measurement** — level, SWR, and signal strength in real-world scenarios
- **Intuitive control** — touch-based interface with visual feedback
- **Flexibility** — multiple operating modes and calibration options
- **Reliability** — proven firmware architecture and stable parameter storage

---

## Key Features

- **Frequency Coverage** — 0.5 MHz to 150 MHz
- **Multiple Measurement Modes** — VOB, SWR, SNA, GDO, STR
- **Touchscreen Interface** — intuitive graphical control
- **Real-Time Sweep Display** — continuous frequency scanning with graphical output
- **Center Frequency & Span Tuning** — adjust measurement range directly from screen
- **Peak Detection and Hold** — capture transient signals
- **SWR Measurement** — antenna matching evaluation
- **Grid-Dip Functionality** — tuned circuit resonance detection
- **Signal Strength Meter** — analog-style indicator display
- **Calibration Menus** — touchscreen calibration for ADC, Si5351, and SWR offset
- **EEPROM Storage** — persistent settings and configuration
- **Battery Status Indicator** — real-time battery level display
- **Portable Design** — compact handheld platform

---

## Hardware Architecture

RF_Sniper-Ver.3 is built on a compact embedded platform optimized for field RF measurement.

| Component | Selection |
|---|---|
| **Microcontroller** | STM32F103CBT6 (ARM Cortex-M3, 72 MHz) |
| **Display** | ILI9341 TFT, 320 × 240 pixels |
| **Touch Interface** | XPT2046 resistive touch controller |
| **Frequency Generator** | Si5351 programmable clock generator |
| **RF Front-End** | Custom analog signal conditioning |
| **ADC** | STM32 internal 12-bit ADC |
| **Storage** | EEPROM 24Cxx series for configuration |
| **Power Supply** | Battery-powered, regulated 3.3V |

### Signal Path

```
RF Input (0.5–150 MHz)
     │
     ▼
Analog Front-End
     │
     ▼
ADC Sampling
     │
     ▼
Measurement Computation
     ├── Level (dBm)
     ├── SWR Calculation
     └── Peak Detection
     │
     ▼
Display Rendering
     │
     ▼
Touch Response
```

---

## Operating Modes

### VOB Mode

Displays transmitted or received signal level across the frequency range. Useful for:

- Power level sweep
- signal presence mapping
- broadband measurement

### SNA Mode

Signal analyzer mode for examining signal strength characteristics. Similar to VOB but with different calibration and display scaling.

### SWR Mode

Specialized mode for antenna tuning and impedance matching. Measures reflection coefficient and converts to SWR for evaluation.

Calibration includes:

- 50-ohm reference calibration
- offset adjustment for return loss
- min/max frequency tracking

### GDO Mode (Grid-Dip Operation)

Analog-style grid-dip meter mode using an analog needle gauge and fine frequency control.

- Single-frequency operation
- visual resonance indication
- precision tuning with UP/DOWN buttons
- toggle between GDO and STRENGTH meter styles

### STR Mode (Strength Meter)

Analog-style signal strength indicator mode with needle gauge and dBm-scaled display.

---

## Software Features

### Graphical Scan Display

- continuous sweep across center ± span
- real-time trace rendering in color
- automatic peak and minimum tracking
- overlay display of min/max frequency and values
- frequency marker for spot measurement

### Touch Interface

- center frequency adjustment via virtual keyboard
- span selection from preset list
- reference level (dB) adjustment
- Y-axis division scaling
- quick preset frequency templates (ham bands)

### Calibration System

On-screen calibration menus allow adjustment of:

- **Touch calibration** — 5-point touchscreen calibration
- **Si5351 crystal correction** — frequency accuracy tuning
- **Local oscillator frequency** — LO adjustment for measurement offset
- **SWR calibration** — return-loss offset for antenna matching

### Settings Storage

All calibration data and user preferences are saved to EEPROM and recalled on power-up.

---

## Measurement Techniques

### Signal Level (dBm)

The firmware reads ADC samples from the RF input and converts them to a dBm-equivalent value using:

```
dBm = (400 * voltage) - 850 (linear approximation)
```

Calibration offset can be applied for accuracy.

### SWR Calculation

SWR is computed from the reflected power measurement using:

```
SWR = (R_antenna + 50) / (R_antenna - 50)
```

where the antenna resistance is derived from the ADC measurement of reflection coefficient.

### Peak Detection

The firmware continuously tracks maximum and minimum signal levels across each frequency sweep, displaying:

- frequency of max/min signal
- absolute level at that frequency
- updated dynamically as the scan progresses

---

## Building and Uploading

### Prerequisites

- Arduino IDE with STM32 board support (stm32duino)
- USB-to-serial programmer or ST-Link debugger
- Required libraries:
  - `Adafruit_ILI9341`
  - `XPT2046_Touchscreen`
  - `si5351` (Si5351 library)
  - `at24c02` (EEPROM library)

### Compilation

1. Install STM32 board package via Arduino Boards Manager
2. Select board: **Generic STM32F1 series**
3. Select variant: **STM32F103CB (20k RAM, 128k Flash)**
4. Configure upload method (USB-serial or ST-Link)
5. Compile and upload the sketch

```bash
# Verify compilation
arduino-cli compile --fqbn STMicroelectronics:stm32:GenF1 RF_Snipper_V3.ino

# Upload to board
arduino-cli upload --fqbn STMicroelectronics:stm32:GenF1 \
  -p /dev/ttyUSB0 RF_Snipper_V3.ino
```

### First Power-Up

On initial startup, the firmware enters the settings menu if the touch screen is held down. Complete:

1. **Touch Calibration** — tap targets at screen corners
2. **Si5351 Calibration** — adjust 10 MHz reference against frequency meter
3. **Local Oscillator Frequency** — set LO offset for your measurement mode
4. **SWR Return-Loss Adjustment** — calibrate with 50-ohm termination
5. Press EXIT to save and restart

---

## Project Structure

```
RF_Sniper-Ver.3/
├── RF_Snipper_V3.ino       # main firmware and initialization
├── Scan.ino                # sweep and measurement loops
├── Display.ino             # TFT rendering and UI
├── TouchScreen.ino         # touch event handling
├── GridDipMeter.ino        # GDO / analog meter mode
├── Settings.ino            # calibration menus
├── Keypad4x4.ino           # virtual keyboard input
├── Push_Keys.ino           # button handling
├── Si5351.ino              # frequency synthesizer control
├── EEprom.ino              # EEPROM read/write
├── Smeter_bitmap.h         # analog meter face bitmap
└── LICENSE                 # GNU GPL v2.0
```

---

## Use Cases

RF_Sniper-Ver.3 is designed for:

- **Antenna Tuning** — visualize impedance across frequency
- **Band Exploration** — scan ham radio bands for activity
- **Signal Investigation** — locate unknown RF sources
- **Frequency Planning** — check interference and occupancy
- **Receiver Testing** — measure RF input levels
- **Transmitter Checkout** — quick frequency and level verification
- **Educational Use** — learning RF measurement principles
- **Portable Testing** — handheld field operations

---

## Development History

RF_Sniper-Ver.3 evolved from the original Antuino project created by Andreas (EB7ME) in 2019.

**Major improvements in this version:**

- STM32F103 processor for improved performance
- ILI9341 color TFT display with full touch control
- redesigned UI for touchscreen operation
- enhanced calibration procedures
- refined measurement algorithms
- improved battery monitoring
- EEPROM-based persistent configuration
- grid-dip meter with analog needle gauge

The project maintains the core concept of a practical, handheld RF measurement tool while incorporating modern embedded design practices.

---

## Project Status

**Version:** 3.5 (as of December 2025)

RF_Sniper-Ver.3 is a functional, field-proven RF measurement instrument and is actively maintained and developed.

The firmware is stable for:

- frequency scanning and display
- signal measurement
- SWR evaluation
- grid-dip operation
- touchscreen interface

Ongoing improvements focus on:

- measurement accuracy refinement
- UI responsiveness
- calibration robustness
- battery efficiency

---

## Roadmap

| Feature | Status |
|---|---|
| Graphical frequency sweep | ✓ Functional |
| Touch interface | ✓ Functional |
| SWR measurement | ✓ Functional |
| Signal level measurement | ✓ Functional |
| Grid-dip meter mode | ✓ Functional |
| Calibration menus | ✓ Functional |
| Frequency preset templates | ✓ Implemented |
| Peak detection and hold | ✓ Implemented |
| Battery level display | ✓ Implemented |
| EEPROM configuration storage | ✓ Implemented |
| Advanced filtering | — Planned |
| Data logging | — Planned |
| Expanded frequency range | — Planned |

---

## Technical Notes

### Frequency Accuracy

Frequency accuracy depends on Si5351 crystal calibration. The firmware includes an on-screen calibration routine using a 10 MHz reference. With proper calibration, frequency error should be <50 ppm across the operating range.

### Measurement Range

- **Signal Level:** approximately −85 dBm to 0 dBm (limited by ADC and front-end)
- **SWR:** 1.0 to 9.99 (limited by return-loss measurement range)
- **Frequency:** 0.5 MHz to 150 MHz (hardware dependent)

### ADC Sampling

The ADC operates in a continuous scan mode with averaging (8 samples per reading) to reduce noise.

### Display Update Rate

The graphical display is updated continuously during a sweep. Update speed depends on:

- span width (larger spans take longer)
- touch interaction
- ADC averaging settings

Typical sweep time: 500 ms to 2 seconds for a full 300-pixel trace.

---

## Credits and Attribution

**Original Concept:** Andreas EB7ME — Antuino project (2019)

**Development & Enhancements:** Ovidiu YO6PIR

RF_Sniper-Ver.3 represents a practical continuation of the Antuino concept, modernizing the platform with contemporary embedded systems techniques while maintaining the core philosophy of a compact, portable RF measurement tool.

---

## License

This project is distributed under the **GNU General Public License v2.0**.

See the [LICENSE](LICENSE) file for full terms and conditions.

---

## References

- **Project Documentation:** https://qsl.net/yo6pir/snipper3.html
- **Original Antuino:** EB7ME concepts and methodology
- **Si5351 Library:** Etherkit Si5351 Arduino Library
- **Display Library:** Adafruit ILI9341 TFT Library
- **Touch Library:** Paul Stoffregen's XPT2046 Touchscreen Library

---

## Support and Contribution

For issues, questions, or contributions, please:

1. Check the [project documentation](https://qsl.net/yo6pir/snipper3.html)
2. Review existing code and comments
3. Open an issue or pull request on GitHub

This is an active amateur radio project. Feedback and improvements are welcome from the community.

---

**Last Updated:** December 2025  
**Firmware Version:** 3.5  
**Status:** Active Development
