# Light Weight: Reactive Nood System

"Light Weight" is an interactive lighting installation that uses a strain sensor to control flexible LED filaments (Noods) via an ESP32-S3. The system translates physical tension into organic light patterns, ranging from soft fades to high-energy "spasms."

## 🚀 Features
- **Strain-Reactive Logic:** Real-time lighting response based on structural tension.
- **Double Smoothing:** 
  - **Sensor Smoothing:** EMA filter to remove HX711 signal noise.
  - **Brightness Smoothing:** Organic attack/decay for "breathing" light effects.
- **Power Management:** 70% brightness cap (PWM 178) to ensure stability on 5V power rails.
- **State-Based Transitions:**
  - **Deadzone:** All lights off below threshold.
  - **Single Fade:** Nood 1 reactive.
  - **Dual Fade:** Nood 1 stays on, Nood 2 becomes reactive.
  - **Spasm Mode:** Erratic, high-speed flickering when max tension is reached.

## 🛠 Hardware Stack
- **Microcontroller:** ESP32-S3 (DevKitC-1)
- **Sensor:** 4-Wire Strain Gauge + HX711 Amplifier
- **LED Drivers:** 3x TPS61169 Constant-Current Boost Drivers (5V to 24V)
- **Light Source:** Adafruit Noods (Flexible LED filaments)
- **Power:** 5V AC Adapter (Minimum 2A recommended)

## 📌 Pin Mapping
| Component | Function | ESP32-S3 GPIO |
| :--- | :--- | :---: |
| **HX711** | Data (DT) | 5 |
| **HX711** | Clock (SCK) | 6 |
| **Driver 1** | Long Nood 1 (Main) | 2 |
| **Driver 2** | Long Nood 2 (T1) | 3 |
| **Driver 3** | Small Noods Chain (T2) | 4 |

## 💻 Software Setup
This project is built using **PlatformIO**.

1. **Install PlatformIO** (VS Code extension or Core CLI).
2. **Library Dependencies:** `bogde/HX711`.
3. **Build & Upload:**
   ```bash
   pio run --target upload
   ```
4. **Monitor Serial:**
   ```bash
   pio device monitor -b 115200
   ```

## ⚠️ Notes
- The TPS61169 drivers are active-high (0 = Off, 255 = Max).
- The current code limits max PWM to `178` to prevent voltage sags on the 5V rail.
- Calibration values (`VAL_0`, `VAL_1`, `VAL_2`) in `main.cpp` should be adjusted based on your specific strain gauge mounting.
