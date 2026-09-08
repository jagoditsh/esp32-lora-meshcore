#ifndef LORA_CONFIG_H
#define LORA_CONFIG_H

// ESP32 Heltec v4 Pin Configuration
// Refer to: https://docs.heltec.org/en/node/esp32/dev-board/hardware-update-log.html

#define LORA_SCK       5       // GPIO5 (SCK)
#define LORA_MISO      19      // GPIO19 (MISO)
#define LORA_MOSI      27      // GPIO27 (MOSI)
#define LORA_CS        18      // GPIO18 (SS/CS)
#define LORA_RST       23      // GPIO23 (RESET)
#define LORA_IRQ       26      // GPIO26 (DIO0/IRQ)

// LoRa Configuration Parameters
#define LORA_FREQUENCY         433E6  // 433 MHz (adjust based on your region)
#define LORA_BANDWIDTH         125E3  // 125 kHz
#define LORA_SPREADING_FACTOR  7      // 7-12 (higher = longer range, slower)
#define LORA_CODING_RATE       4      // 5-8
#define LORA_PREAMBLE_LENGTH   8
#define LORA_TX_POWER          20     // dBm (max 20)
#define LORA_SYNC_WORD         0x34   // Public network sync word

// Mesh Configuration
#define MESH_NETWORK_ID        42     // Network identifier
#define MESH_NODE_ID           1      // This device's node ID (change for each device)
#define MESH_HOP_LIMIT         3      // Maximum hops for mesh routing

// Timeouts and intervals
#define MESH_HEARTBEAT_INTERVAL 30000  // 30 seconds
#define MESSAGE_TIMEOUT         5000   // 5 seconds

#endif // LORA_CONFIG_H
