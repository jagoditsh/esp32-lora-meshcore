/*
 * Multi-Node Mesh Example
 * 
 * This example demonstrates how to configure multiple devices as nodes
 * in a LoRa mesh network. Each device can send/receive messages and
 * relay packets for other nodes.
 * 
 * Setup:
 * - Device 1: MESH_NODE_ID = 1
 * - Device 2: MESH_NODE_ID = 2
 * - Device 3: MESH_NODE_ID = 3
 * 
 * All devices must have identical LoRa parameters but different node IDs.
 * Change MESH_NODE_ID in lora_config.h for each device.
 */

#include <Arduino.h>
#include <LoRa.h>
#include "lora_config.h"
#include "mesh_manager.h"

MeshManager meshManager;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.printf("\n=== Node %d Initializing ===\n", MESH_NODE_ID);
  
  if (!meshManager.initLoRa()) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  
  LoRa.onReceive(onLoRaReceived);
  LoRa.receive();
  
  Serial.printf("Node %d ready. Network ID: %d\n", MESH_NODE_ID, MESH_NETWORK_ID);
}

void onLoRaReceived(int packetSize) {
  MeshPacket packet;
  if (meshManager.receiveMessage(packet)) {
    meshManager.handleMeshPacket(packet);
  }
}

void loop() {
  // Periodically send status broadcast
  static unsigned long lastBroadcast = 0;
  if (millis() - lastBroadcast > 10000) {
    String statusMsg = "Node " + String(MESH_NODE_ID) + " alive";
    meshManager.broadcastMessage(statusMsg);
    lastBroadcast = millis();
  }
  
  delay(100);
}
