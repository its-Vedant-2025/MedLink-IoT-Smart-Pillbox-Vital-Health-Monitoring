/*
 * Project Topic: IoT Based Smart Pillbox & Vital Health Monitoring
 * App Name: MedLink IoT
 * Created By: Group 51-55 (TCET)
 *
 * Description:
 * This firmware controls the servo motor for pill dispensing, reads
 * real-time health data from the MAX30102 sensor, and updates the
 * OLED display. It communicates with the cloud via ESP32.
 */

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "heartRate.h"

// --- Configuration ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;
Servo dispenserServo;

// --- Pins ---
const int SERVO_PIN = 9;
const int IR_SENSOR_PIN = 7;
const int BUZZER_PIN = 8;

// --- Variables ---
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;
bool isPillDue = false;

void setup() {
  Serial.begin(9600); // Start Serial Communication

  // Initialize Components
  dispenserServo.attach(SERVO_PIN);
  dispenserServo.write(0); // Set to initial position

  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("MedLink IoT Initializing...");
  display.display();
  delay(2000);

  // Initialize Pulse Sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 not found. Check wiring.");
    while (1);
  }
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
}

void loop() {
  // 1. Read Health Data
  long irValue = particleSensor.getIR();
  if (checkForBeat(irValue) == true) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      beatAvg = (int)beatsPerMinute;
    }
  }

  // 2. Read Commands from App (via ESP32)
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "DISPENSE") {
      dispensePill();
    }
  }

  // 3. Update OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("App: MedLink IoT");

  display.setCursor(0, 20);
  display.print("Status: ");
  display.println(isPillDue ? "TAKE PILL!" : "Monitoring...");

  display.setCursor(0, 40);
  display.print("BPM: ");
  display.println(beatAvg);

  display.display();

  // 4. Send Data to App
  // Format: BPM,SpO2,Status
  Serial.print(beatAvg);
  Serial.print(",");
  Serial.print(98); // SpO2 simulated for stable demo
  Serial.print(",");
  Serial.println(isPillDue ? "1" : "0");

  delay(100);
}

void dispensePill() {
  isPillDue = true;
  digitalWrite(BUZZER_PIN, HIGH);

  // Rotate Drum
  dispenserServo.write(45); // Rotate to drop pill
  delay(1000);
  dispenserServo.write(0); // Return to start

  // Verify Drop with IR Sensor
  if (digitalRead(IR_SENSOR_PIN) == LOW) {
    Serial.println("CONFIRMED_DROP");
    isPillDue = false;
    digitalWrite(BUZZER_PIN, LOW);
  }
}
