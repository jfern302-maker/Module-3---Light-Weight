# Session Summary: Reactive Noods (Strain Sensor Mode)
**Date:** Sunday, April 12, 2026

## 1. Project Overview
The project has been transitioned from a potentiometer-based trigger to a **4-wire Strain Sensor** using an **HX711 Amplifier** and an **ESP32-S3**.

## 2. Hardware Configuration
- **Microcontroller:** ESP32-S3
- **Amplifier:** HX711
- **Sensor:** 4-wire Strain Gauge (Red: E+, Black: E-, White: A-, Green: A+)
- **Outputs:**
  - **Nood 1 (Main):** GPIO 2 (NPN Transistor - Active High)
  - **Nood 2 (T1):** GPIO 3 (PNP Transistor - Active Low)
  - **Nood 3 (T2):** GPIO 4 (PNP Transistor - Active Low)

### HX711 Pinout:
- **DT (Data):** GPIO 5
- **SCK (Clock):** GPIO 6
- **VCC:** 3.3V (or 5V if needed for excitation)
- **GND:** GND

## 3. Software Features
- **Library:** `bogde/HX711` added to `platformio.ini`.
- **Sequential Triggering:**
  - `Below -7,000`: All Off
  - `-7,000 to 30,000`: Nood 1 Fades In
  - `30,000 to 47,000`: Nood 2 Fades In (Nood 1 stays ON)
  - `Above 47,000`: Spasm Mode (Both Noods blink erratically)
- **Double Smoothing:**
  - **Sensor Smoothing (`0.25`):** Exponential Moving Average (EMA) to remove sensor jitter.
  - **Brightness Smoothing (`0.20`):** Creates organic "attack/decay" fades.
- **PNP Fix:** Implemented `round()` and "snap-to-limit" logic to ensure PNP transistors turn fully OFF at 255.
- **Performance:** Refresh rate optimized with `1ms` loop delay and `20ms` HX711 timeout.

## 4. Current State
- **Stability:** Communication with HX711 is stable.
- **Reactivity:** Fast and smooth transitions.
- **Serial Monitor:** Enabled (115200 baud) for monitoring Raw vs. Smooth values.

## 5. Next Steps
- Fine-tune `VAL_0` to `VAL_100` if physical strain limits change.
- Adjust `sensorSmoothingFactor` (0.01 - 1.0) to balance noise vs. speed.
- Consider adding more Noods or patterns.
