# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [0.1.1] - 2025-12-03

### Added
- Template file `secrets_exemple.h` for WiFi credentials
- README instructions to copy the template to `secrets.h`

### Changed
- Improved setup process with credentials template
- Documentation enriched with security notes

## [0.1.0] - 2025-12-03

### Added
- Initial release of ESP32-Test
- Web-based system diagnostic interface
- Real-time hardware information display
- Support for ESP32-S3 (N8R8 / N16R8) and ESP32 Classic
- Chip info display (model, cores, frequency, temperature)
- Memory monitoring (Heap and PSRAM with usage bars)
- Flash details (size in MB, speed, mode)
- Automatic module detection (N8R8/N16R8 reference)
- Complete flash partition table viewer
- Network info (WiFi SSID, IP, MAC, RSSI)
- Multi-network support with WiFiMulti
- mDNS support for access via `ESP32-Test.local`
- JSON API endpoint at `/api/data`
- Responsive web interface with auto-refresh (10s)
- Three pre-configured PlatformIO environments:
	- `esp32s3_n16r8`: ESP32-S3 with 16MB Flash / 8MB PSRAM
	- `esp32s3_n8r8`: ESP32-S3 with 8MB Flash / 8MB PSRAM
	- `esp32devkitc`: ESP32 Classic with 4-8MB Flash
- Semantic version display in the web interface
- Complete documentation (EN/FR)

### Technical Details
- Platform: ESP32 Arduino Core (ESP-IDF v5.1.2+)
- Framework: Arduino
- Build system: PlatformIO
- Web server: ESPAsyncWebServer
- Memory: Heap + PSRAM support
- Flash: QIO/DIO mode detection
- USB CDC support for ESP32-S3

### Documentation
- README.md (English)
- README_FR.md (French)
- CHANGELOG.md (English)
- CHANGELOG_FR.md (French)
- API reference documentation
- Hardware configuration guide
- Installation and setup instructions

## [Unreleased]

### Planned Features
- OTA firmware update capability
- Historical data logging and charts
- Configuration via web interface
- MQTT integration for IoT platforms
- Extended REST API for external tools
- Mobile companion app
- Multi-language interface support
- Custom themes and dark mode
- Export diagnostic reports (JSON/CSV)
- Advanced network diagnostics

---

[0.1.1]: https://github.com/yourusername/ESP32-Test/releases/tag/v0.1.1
[0.1.0]: https://github.com/yourusername/ESP32-Test/releases/tag/v0.1.0
