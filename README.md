# ESP32 Heltec v4 LoRa Mesh Communication

This is a PlatformIO-based project for ESP32 Heltec v4 boards that implements LoRa mesh networking for point-to-point and broadcast messaging through intermediate nodes.

## Hardware

- **Microcontroller**: ESP32 (Heltec WiFi LoRa 32 v4)
- **LoRa Module**: Built-in SX1276/SX1278
- **Display**: OLED 0.96" SSD1306
- **Frequency Range**: 433 MHz (configurable)

## Features

- ✅ LoRa point-to-point messaging
- ✅ Mesh routing with hop-limit support
- ✅ Broadcast messaging
- ✅ OLED display feedback
- ✅ Serial command interface
- ✅ RSSI and SNR monitoring
- ✅ CRC error detection

## Pin Configuration

| Function | GPIO | Notes |
|----------|------|-------|
| SCK      | 5    | SPI Clock |
| MISO     | 19   | SPI Master In Slave Out |
| MOSI     | 27   | SPI Master Out Slave In |
| CS       | 18   | Chip Select |
| RST      | 23   | LoRa Reset |
| IRQ/DIO0 | 26   | Interrupt Request |

## Installation

### Prerequisites

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)
3. Python 3.6+ (installed with PlatformIO)

### Setup

1. Clone this repository:
   ```bash
   git clone https://github.com/jagoditsh/esp32-lora-meshcore.git
   cd esp32-lora-meshcore
   ```

2. Open the project in VS Code

3. Connect your ESP32 Heltec v4 board via USB

4. Build and upload:
   ```bash
   PlatformIO: Upload (Ctrl+Alt+U)
   ```

## Serial Commands

Open the Serial Monitor (Ctrl+Alt+S) and use these commands:

### Send a Message
```
send:Hello from Node 1
```

### Display System Status
```
status
```

### Show Help
```
help
```

## Configuration

Edit `src/lora_config.h` to customize:

- **LORA_FREQUENCY**: Operating frequency (default: 433 MHz)
- **LORA_SPREADING_FACTOR**: 7-12 (higher = longer range, slower)
- **LORA_BANDWIDTH**: Bandwidth in Hz (125k, 250k, 500k)
- **LORA_TX_POWER**: Transmission power (0-20 dBm)
- **MESH_NODE_ID**: Unique identifier for each device
- **MESH_HOP_LIMIT**: Maximum mesh hops

## Mesh Protocol

### Packet Format
```
DestinationID | SourceID | HopsLeft | MessageID | Payload
```

### Packet Fields

- **DestinationID**: Target node (255 = broadcast)
- **SourceID**: Originating node
- **HopsLeft**: Remaining relay hops
- **MessageID**: Unique message identifier
- **Payload**: Message content (UTF-8)

### Routing Behavior

1. **Direct Message**: Device sends to specific node ID
2. **Broadcast**: Device sends to ID 255 (all nodes)
3. **Mesh Relay**: Intermediate nodes relay if hops remaining
4. **Local Delivery**: Message consumed if destination ID matches own node ID

## Troubleshooting

### LoRa not initializing
- Verify USB connection
- Check pin configuration in `lora_config.h`
- Ensure SPI bus is not used by other devices
- Check board selection in `platformio.ini`

### No messages received
- Verify both devices have same frequency and spreading factor
- Check antenna connection
- Monitor RSSI and SNR values
- Increase TX power if devices are far apart
- Check hops remaining for mesh messages

### OLED display not showing
- Verify I2C address (default: 0x3C)
- Check SDA/SCL connections
- Look for initialization errors in Serial Monitor

## Monitoring

Serial output example:
```
=== ESP32 Heltec v4 LoRa Mesh Communication ===
Initializing...
Initializing LoRa...
LoRa initialized:
  Frequency: 433000000 Hz
  Spreading Factor: 7
  Bandwidth: 125000 Hz
  TX Power: 20 dBm
[MESH TX] To: 2, From: 1, ID: 1, Data: Hello Node 2
[MESH RX] From: 2, ID: 1, RSSI: -85, SNR: 8.50
  Payload: Hello back Node 1
```

## Resources

- [Heltec ESP32 Documentation](https://docs.heltec.org/)
- [LoRa Arduino Library](https://github.com/sandeepmistry/arduino-LoRa)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [LoRa Modulation Basics](https://lora-alliance.org/)

## License

MIT License - feel free to use in your projects

## Contributing

Contributions welcome! Please submit pull requests for improvements.
