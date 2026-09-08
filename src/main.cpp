#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "lora_config.h"
#include "mesh_manager.h"

// OLED Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Global mesh manager
MeshManager meshManager;

// Message counter
unsigned long messageCount = 0;

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Halt
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("LoRa Mesh Initialize");
  display.display();
}

void updateDisplay(const String &message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 Heltec LoRa Mesh");
  display.println("---");
  display.println(message);
  display.println("---");
  display.print("Messages: ");
  display.println(messageCount);
  display.display();
}

void onLoRaReceived() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedMessage = "";
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    Serial.printf("[RX] Message: %s | RSSI: %d | SNR: %.2f\n", receivedMessage.c_str(), rssi, snr);
    updateDisplay("RX: " + receivedMessage);
  }
}

void sendLoRaMessage(const String &message) {
  Serial.printf("[TX] Sending: %s\n", message.c_str());
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  messageCount++;
  updateDisplay("TX: " + message);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== ESP32 Heltec v4 LoRa Mesh Communication ===");
  Serial.println("Initializing...");

  // Initialize display
  initDisplay();
  delay(500);

  // Initialize LoRa
  Serial.println("Initializing LoRa...");
  if (!meshManager.initLoRa()) {
    Serial.println("LoRa initialization failed!");
    updateDisplay("LoRa Init Failed");
    while (1);
  }
  
  Serial.println("LoRa initialized successfully!");
  updateDisplay("LoRa Ready");
  delay(2000);

  // Set up LoRa callback for receiving
  LoRa.onReceive(onLoRaReceived);
  LoRa.receive();

  Serial.println("System ready. Waiting for messages...");
}

void loop() {
  // Check for incoming serial commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.length() > 0) {
      if (command.startsWith("send:")) {
        String message = command.substring(5);
        sendLoRaMessage(message);
      } else if (command == "status") {
        Serial.printf("Messages sent: %lu\n", messageCount);
        Serial.printf("LoRa Frequency: %u Hz\n", LORA_FREQUENCY);
        Serial.printf("LoRa Bandwidth: %u Hz\n", LORA_BANDWIDTH);
        Serial.printf("LoRa Spreading Factor: %d\n", LORA_SPREADING_FACTOR);
      } else if (command == "help") {
        Serial.println("Available commands:");
        Serial.println("  send:<message> - Send a LoRa message");
        Serial.println("  status - Display system status");
        Serial.println("  help - Show this help message");
      }
    }
  }

  // Small delay to prevent watchdog timeout
  delay(100);
}
