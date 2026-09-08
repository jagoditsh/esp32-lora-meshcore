#include "mesh_manager.h"
#include <SPI.h>

MeshManager::MeshManager() : lastMessageId(0) {}

bool MeshManager::initLoRa() {
  // Initialize SPI
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  // Initialize LoRa
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa init failed. Check your connections.");
    return false;
  }

  // Configure LoRa parameters
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setSignalBandwidth(LORA_BANDWIDTH);
  LoRa.setCodingRate4(LORA_CODING_RATE);
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa.setTxPower(LORA_TX_POWER);
  LoRa.setSyncWord(LORA_SYNC_WORD);
  LoRa.enableCrc();

  Serial.println("LoRa initialized:");
  Serial.printf("  Frequency: %u Hz\n", LORA_FREQUENCY);
  Serial.printf("  Spreading Factor: %d\n", LORA_SPREADING_FACTOR);
  Serial.printf("  Bandwidth: %u Hz\n", LORA_BANDWIDTH);
  Serial.printf("  TX Power: %d dBm\n", LORA_TX_POWER);

  return true;
}

void MeshManager::sendMessage(uint8_t destinationId, const String &message) {
  MeshPacket packet;
  packet.destinationId = destinationId;
  packet.sourceId = MESH_NODE_ID;
  packet.hopsLeft = MESH_HOP_LIMIT;
  packet.messageId = ++lastMessageId;
  packet.payload = message;

  // Encode and send packet
  String packetData = String(packet.destinationId) + "|" +
                     String(packet.sourceId) + "|" +
                     String(packet.hopsLeft) + "|" +
                     String(packet.messageId) + "|" +
                     packet.payload;

  Serial.printf("[MESH TX] To: %d, From: %d, ID: %u, Data: %s\n",
                packet.destinationId, packet.sourceId, packet.messageId, message.c_str());

  LoRa.beginPacket();
  LoRa.print(packetData);
  LoRa.endPacket();
}

void MeshManager::broadcastMessage(const String &message) {
  sendMessage(255, message); // 255 = broadcast address
}

bool MeshManager::receiveMessage(MeshPacket &packet) {
  int packetSize = LoRa.parsePacket();
  if (packetSize == 0) return false;

  String packetData = "";
  while (LoRa.available()) {
    packetData += (char)LoRa.read();
  }

  // Parse packet format: destId|srcId|hopsLeft|msgId|payload
  int firstPipe = packetData.indexOf('|');
  int secondPipe = packetData.indexOf('|', firstPipe + 1);
  int thirdPipe = packetData.indexOf('|', secondPipe + 1);
  int fourthPipe = packetData.indexOf('|', thirdPipe + 1);

  if (firstPipe == -1 || secondPipe == -1 || thirdPipe == -1 || fourthPipe == -1) {
    return false; // Invalid packet format
  }

  packet.destinationId = packetData.substring(0, firstPipe).toInt();
  packet.sourceId = packetData.substring(firstPipe + 1, secondPipe).toInt();
  packet.hopsLeft = packetData.substring(secondPipe + 1, thirdPipe).toInt();
  packet.messageId = packetData.substring(thirdPipe + 1, fourthPipe).toInt();
  packet.payload = packetData.substring(fourthPipe + 1);

  return true;
}

void MeshManager::handleMeshPacket(const MeshPacket &packet) {
  // Check if packet is for this node or broadcast
  if (packet.destinationId != MESH_NODE_ID && packet.destinationId != 255) {
    // Relay packet if hops available
    if (packet.hopsLeft > 1) {
      Serial.printf("[MESH RELAY] Relaying packet %u to node %d\n",
                    packet.messageId, packet.destinationId);
      // Create modified packet with decreased hop count
      // Re-send logic would go here
    }
  } else {
    // Packet is for us
    Serial.printf("[MESH RX] From: %d, ID: %u, RSSI: %d, SNR: %.2f\n",
                  packet.sourceId, packet.messageId, LoRa.packetRssi(), LoRa.packetSnr());
    Serial.printf("  Payload: %s\n", packet.payload.c_str());
  }
}

void MeshManager::printPacketInfo(const MeshPacket &packet) {
  Serial.println("=== Mesh Packet ===");
  Serial.printf("Destination ID: %d\n", packet.destinationId);
  Serial.printf("Source ID: %d\n", packet.sourceId);
  Serial.printf("Hops Left: %d\n", packet.hopsLeft);
  Serial.printf("Message ID: %u\n", packet.messageId);
  Serial.printf("Payload: %s\n", packet.payload.c_str());
}
