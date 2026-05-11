# Wiring Guide: Reactive Noods (ESP32-S3 + TPS61169 Drivers)

This project uses an ESP32-S3 and three TPS61169 constant-current drivers to control multiple Adafruit Noods based on strain sensor data.

## 1. Components
- **ESP32-S3** (e.g., DevKitC-1)
- **3x Adafruit TPS61169 LED Drivers**
- **2x Long Noods** (Flexible LED filaments)
- **Series Chain of Small Noods**
- **HX711 Amplifier + 4-Wire Strain Sensor**
- **5V AC Adapter** (for power)

---

## 2. Power Distribution (Main Rails)
- **5V Rail:** Connect AC Adapter (+), ESP32-S3 **5V/VBUS** pin, and all **TPS61169 VIN** pins.
- **GND Rail:** Connect AC Adapter (-), ESP32-S3 **GND**, and all **TPS61169 GND** pins.

---

## 3. Sensor Wiring (Input)
### HX711 to ESP32-S3
| HX711 Pin | ESP32-S3 Pin |
| :--- | :--- |
| VCC | 3.3V |
| GND | GND |
| DT (Data) | GPIO 5 |
| SCK (Clock)| GPIO 6 |

### Strain Sensor to HX711
- **Red:** E+
- **Black:** E-
- **White:** A-
- **Green:** A+

---

## 4. Noods Wiring (Output via Drivers)

The TPS61169 drivers act as constant-current sources. Connect the **CTRL** pin of each driver to the ESP32-S3.

| Driver | Function | ESP32 Pin (CTRL) | Output (LED+/LED-) |
| :--- | :--- | :---: | :--- |
| **Driver 1** | Long Nood 1 | GPIO 2 | To Nood 1 |
| **Driver 2** | Long Nood 2 | GPIO 3 | To Nood 2 |
| **Driver 3** | Small Noods Chain | GPIO 4 | To Series Chain |

### Series Chain (Driver 3)
Connect the small Noods end-to-end:
1. Driver 3 **LED+** -> First Small Nood **(+)**
2. First Small Nood **(-)** -> Second Small Nood **(+)**
3. ... (continue for all small Noods)
4. Last Small Nood **(-)** -> Driver 3 **LED-**

---

## 5. Notes for Soldering
- **Common Ground:** Ensure the AC adapter ground is tied to the ESP32 ground.
- **Current Setting:** The TPS61169 current is set by a resistor on the breakout. Ensure it is configured for your Noods (typically ~20-50mA).
- **PWM Logic:** The code now uses standard 0-255 PWM (Active-High).
