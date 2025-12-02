# ESP32-Test

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-compatible-blue.svg)](https://platformio.org/)
[![Version](https://img.shields.io/badge/version-0.1.2-green.svg)](CHANGELOG.md)

**ESP32-Test** is a lightweight web-based system diagnostic tool for ESP32 microcontrollers. It provides real-time hardware information through a clean web interface, perfect for testing and monitoring ESP32-S3, ESP32-S2, and ESP32 Classic boards.

[🇫🇷 Version française](README_FR.md)

## ✨ Features

- 🌐 **Web Interface**: Clean, responsive dashboard accessible via WiFi
- 🔍 **System Information**: Chip model, cores, frequency, temperature
- 💾 **Memory Monitoring**: Heap and PSRAM usage with visual indicators
- 📊 **Flash Details**: Total size (8/16 MB), speed, mode detection
- 🗂️ **Partition Table**: Complete overview of flash partitions
- 🔌 **Network Info**: WiFi SSID, IP, MAC addresses, RSSI
- 🔄 **Multi-Network**: Automatic connection to best available WiFi
- 📱 **mDNS Support**: Access via `http://ESP32-Test.local`
- 🏷️ **Module Detection**: Automatic N8R8/N16R8 reference display

## 🖼️ Screenshot

The web interface displays all critical system information in organized cards with real-time updates every 10 seconds.

## 🛠️ Hardware Support

### Tested Boards
- ✅ ESP32-S3-DevKitC-1 (N8R8 / N16R8)
- ✅ Freenove ESP32-S3 WROOM (N8R8)
- ✅ ESP32 Classic (DevKitC)

### Specifications
| Model | Flash | PSRAM | USB CDC | Status |
|-------|-------|-------|---------|--------|
| ESP32-S3 N16R8 | 16 MB | 8 MB | ✅ | Tested |
| ESP32-S3 N8R8 | 8 MB | 8 MB | ✅ | Tested |
| ESP32 Classic | 4-8 MB | ❌ | ❌ | Compatible |

See [HARDWARE.md](docs/HARDWARE.md) for detailed pin configurations.

## 📦 Installation

### Prerequisites
- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- ESP32 board (see supported models above)
- USB cable for programming

### Quick Start

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/ESP32-Test.git
cd ESP32-Test
```

2. **Configure WiFi credentials**

Copy the template file and edit with your credentials:
```bash
# Copy template
cp include/secrets_exemple.h include/secrets.h

# Edit with your WiFi settings
```

Edit `include/secrets.h`:
```cpp
const char* WIFI_SSID_1 = "your_wifi_ssid";
const char* WIFI_PASS_1 = "your_wifi_password";

// Optional: Add second network
const char* WIFI_SSID_2 = "backup_network";
const char* WIFI_PASS_2 = "backup_password";
```

> ⚠️ **Note**: `secrets.h` is gitignored to protect your credentials.

3. **Select your board environment**

Edit `platformio.ini` and set your board:
```ini
[platformio]
default_envs = esp32s3_n16r8  ; or esp32s3_n8r8, esp32devkitc
```

4. **Build and upload**
```bash
# Using PlatformIO CLI
platformio run -t upload

# Or use auto detection helper
python scripts/auto_env.py upload

# Or use PlatformIO IDE: click "Upload" button
```

5. **Monitor serial output**
```bash
platformio device monitor -b 115200

# Or auto-detect port
python scripts/auto_env.py monitor
```

6. **Access the web interface**
- Via IP: Check serial monitor for IP address
- Via mDNS: `http://ESP32-Test.local`

## 🔧 Configuration

### Environments

The project includes three pre-configured environments:

#### ESP32-S3 N16R8 (16MB Flash / 8MB PSRAM)
```ini
[env:esp32s3_n16r8]
platform = espressif32@^6.5.0
board = esp32-s3-devkitc-1
board_build.flash_size = 16MB
board_build.psram = enabled
```

#### ESP32-S3 N8R8 (8MB Flash / 8MB PSRAM)
```ini
[env:esp32s3_n8r8]
board_build.flash_size = 8MB
board_build.psram = enabled
```

#### ESP32 Classic (4MB Flash)
```ini
[env:esp32devkitc]
board = esp32dev
board_build.flash_size = 4MB
```

### Customization

- **Hostname**: Edit `HOSTNAME` in `src/main.cpp`
- **Monitor port**: Change `monitor_speed` in `platformio.ini`
- **Upload speed**: Adjust `upload_speed` if needed

## 📡 API Reference

The web interface consumes a JSON API at `/api/data`:

```json
{
  "chip": {
    "model": "ESP32-S3",
    "cores": 2,
    "revision": 0,
    "freq": 240
  },
  "sys": {
    "sdk": "v3.3.4",
    "idf": "v5.1.2",
    "app_version": "0.1.0"
  },
  "mem": {
    "heap_total": 327680,
    "heap_free": 280672,
    "psram_total": 8388608,
    "psram_free": 8388608
  },
  "flash": {
    "size": 16777216,
    "size_mb": 16.0,
    "speed": 80000000,
    "mode": "QIO"
  },
  "module": {
    "ref": "N16R8"
  }
}
```

See [API.md](docs/API.md) for complete documentation.

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Built with [PlatformIO](https://platformio.org/)
- ESP32 Arduino Core by Espressif Systems
- Web interface inspired by modern dashboard designs

## 📞 Support

- 📖 [Documentation](docs/)
- 🐛 [Issues](https://github.com/yourusername/ESP32-Test/issues)
- 💬 [Discussions](https://github.com/yourusername/ESP32-Test/discussions)

## 🗺️ Roadmap

- [ ] OTA firmware updates
- [ ] Historical data charts
- [ ] Configuration via web interface
- [ ] MQTT integration
- [ ] REST API for external tools
- [ ] Mobile app companion

---

Made with ❤️ for the ESP32 community
