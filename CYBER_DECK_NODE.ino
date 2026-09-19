// ============================================================================
// Project: Arduino-CyberDeck-Telemetry-Node
// Author & Architect: Pranjal Das
// Copyright (c) 2026 Pranjal Das. All Rights Reserved.
// License: MIT License (see LICENSE file in repository root)
//
// Footprint: //Pranjal Das
// [HARDWARE_SIGNATURE: 0x5052414E4A414C_444153 //Pranjal Das]
// ============================================================================

// 🔐 REQUIRED BY BLYNK IoT (don’t remove)
#define BLYNK_TEMPLATE_ID "TMPL_XXXXXX"
#define BLYNK_TEMPLATE_NAME "CyberDeckNode"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

// 🔌 CORE LIBRARIES
#include <ESP8266WiFi.h>              // WiFi support for NodeMCU
#include <BlynkSimpleEsp8266.h>       // Blynk library for ESP8266

// 📶 WiFi credentials — replace with YOUR network info
char ssid[] = "YOUR_WIFI_SSID";         // 👈 your WiFi SSID (name)
char pass[] = "YOUR_WIFI_PASSWORD";     // 👈 your WiFi password

// 🧪 SENSOR: MQ2 Gas Sensor is connected to Analog pin A0
#define GAS_SENSOR A0

// 💡 OUTPUTS: Define safe GPIO pins for 3 LEDs
#define LED1 D1   // GPIO5
#define LED2 D2   // GPIO4
#define LED3 D5   // GPIO14

// 🔌 OUTPUTS: Define safe GPIO pins for 2 relays
#define RELAY1 D6 // GPIO12
#define RELAY2 D7 // GPIO13

// 🕒 Timer for scheduling tasks like gas reading
BlynkTimer timer;

// 📤 Function to read gas sensor and send to Blynk app (runs every 1 sec)
void sendGasData() {
  int gasValue = analogRead(GAS_SENSOR);   // Read analog input from MQ2
  Blynk.virtualWrite(V0, gasValue);        // Send value to Blynk Gauge (V0)

  // 🚨 Optional: Trigger Blynk alert if gas level is dangerous
  if (gasValue > 400) {
    Blynk.logEvent("gas_alert", "⚠️ Gas Level High!");
  }

  // Debug info via Serial Monitor
  Serial.print("Gas Level: ");
  Serial.println(gasValue);
}

// 🔘 Remote toggle from Blynk Switches → LEDs
BLYNK_WRITE(V2) { digitalWrite(LED1, param.asInt()); }  // LED1 control
BLYNK_WRITE(V3) { digitalWrite(LED2, param.asInt()); }  // LED2 control
BLYNK_WRITE(V4) { digitalWrite(LED3, param.asInt()); }  // LED3 control

// 🔘 Remote toggle from Blynk Switches → Relays
BLYNK_WRITE(V5) { digitalWrite(RELAY1, param.asInt()); } // Relay1 control
BLYNK_WRITE(V6) { digitalWrite(RELAY2, param.asInt()); } // Relay2 control

// 🔁 Setup runs once on boot
void setup() {
  Serial.begin(115200);           // For Serial Monitor debugging
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Connect to Blynk IoT

  // Set all pinModes
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Ensure all outputs are OFF at boot for safety
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  // Start repeating gas reading every 1 second
  timer.setInterval(1000L, sendGasData);
}

// 🔁 Main loop keeps Blynk & timer running
void loop() {
  Blynk.run();       // Keep Blynk connection alive
  timer.run();       // Run scheduled tasks like gas reading
}


// Author Footprint: //Pranjal Das
