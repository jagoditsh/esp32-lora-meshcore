#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include <Arduino.h>
#include <LoRa.h>
#include "lora_config.h"

struct MeshPacket {
  uint8_t destinationId;
  uint8_t sourceId;
  uint8_t hopsLeft;
  uint16_t messageId;
  String payload;
};

class MeshManager {
private:
  uint16_t lastMessageId;
  
public:
  MeshManager();
  bool initLoRa();
  void sendMessage(uint8_t destinationId, const String &message);
  void broadcastMessage(const String &message);
  bool receiveMessage(MeshPacket &packet);
  void handleMeshPacket(const MeshPacket &packet);
  void printPacketInfo(const MeshPacket &packet);
};

#endif // MESH_MANAGER_H
