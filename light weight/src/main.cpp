#include <Arduino.h>
#include "HX711.h"

/*
 * Project: ESP32-S3 Reactive Noods (Smooth Strain Sensor Mode)
 */

// HX711 circuit wiring (Matched to WIRING_GUIDE.md)
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;

const int mainNoodPin = 2;  // Driver 1 (Long Nood 1) on GPIO 2
const int t1LightPin = 3;   // Driver 2 (Long Nood 2) on GPIO 3
const int t2LightPin = 4;   // Driver 3 (Small Noods) on GPIO 4

// User Defined Thresholds (Raw Readings)
const long VAL_0   = -7000;  // Everything below this is OFF
const long VAL_1   = 30000; // Nood 1 Full, Nood 2 starts
const long VAL_2   = 47000; // Both Full, Spasm starts

// --- Smoothing Constants ---
float smoothedReading = VAL_0;
const float sensorSmoothingFactor = 0.25; 

float b1Smooth = 0;
float b2Smooth = 0; 
const float brightSmoothingFactor = 0.20;

void setup() {
  Serial.begin(115200);
  delay(1000);

  analogWriteResolution(8);

  pinMode(mainNoodPin, OUTPUT);
  pinMode(t1LightPin, OUTPUT);
  pinMode(t2LightPin, OUTPUT);

  // Initialize all drivers to OFF (Active High)
  analogWrite(mainNoodPin, 0);
  analogWrite(t1LightPin, 0);
  analogWrite(t2LightPin, 0);

  Serial.println("Initializing the scale...");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  if (scale.wait_ready_timeout(500)) {
    smoothedReading = (float)scale.get_value(5);
  }

  Serial.println("Dual Nood Spasm Mode Started.");
}

// --- Brightness Limits ---
const int MAX_BRIGHT = 178; // 70% of 255

void loop() {
  if (scale.wait_ready_timeout(20)) { 
    long rawReading = scale.get_value(1); 
    
    smoothedReading = (rawReading * sensorSmoothingFactor) + (smoothedReading * (1.0 - sensorSmoothingFactor));
    
    float reading = smoothedReading;
    int targetB1 = 0;
    int targetB2 = 0;

    // --- State Logic ---
    if (reading <= VAL_0) {
      // STATE 1: DEADZONE
      targetB1 = 0;
      targetB2 = 0;
    } 
    else if (reading <= VAL_1) {
      // STATE 2: NOOD 1 FADES
      targetB1 = map(reading, VAL_0, VAL_1, 0, MAX_BRIGHT);
      targetB2 = 0;
    } 
    else if (reading <= VAL_2) {
      // STATE 3: NOOD 2 FADES
      targetB1 = MAX_BRIGHT;
      targetB2 = map(reading, VAL_1, VAL_2, 0, MAX_BRIGHT);
    } 
    else {
      // STATE 4: SPASM MODE
      if ((millis() / 30) % 2 == 0) {
        targetB1 = MAX_BRIGHT;
        targetB2 = 40; // Proportional dimming
      } else {
        targetB1 = 40;
        targetB2 = MAX_BRIGHT;
      }
    }

    // Apply smoothing only to the fading states (prevents smoothing out the spasm)
    if (reading <= VAL_2) {
      b1Smooth = (targetB1 * brightSmoothingFactor) + (b1Smooth * (1.0 - brightSmoothingFactor));
      b2Smooth = (targetB2 * brightSmoothingFactor) + (b2Smooth * (1.0 - brightSmoothingFactor));
    } else {
      b1Smooth = targetB1; // Instant response for spasm
      b2Smooth = targetB2;
    }

    // Snap to limits
    if (abs(b1Smooth - targetB1) < 0.5) b1Smooth = targetB1;
    if (abs(b2Smooth - targetB2) < 0.5) b2Smooth = targetB2;

    // Output to LED Drivers (Active High)
    analogWrite(mainNoodPin, (int)round(b1Smooth));
    analogWrite(t1LightPin, (int)round(b2Smooth));

    Serial.print("Raw: "); Serial.print(rawReading);
    Serial.print(" | Smooth: "); Serial.print(reading);
    Serial.print(" | B1: "); Serial.print((int)round(b1Smooth));
    Serial.print(" | B2: "); Serial.println((int)round(b2Smooth));
  }

  delay(1);
}
