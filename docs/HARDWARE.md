# Hardware Configuration Guide

## Supported Boards

### ESP32-S3-DevKitC-1

ESP32-S3-DevKitC-1 is Espressif's official development board featuring the powerful dual-core ESP32-S3 with WiFi and BLE support.

#### Variants

##### ESP32-S3 N16R8 (16MB Flash / 8MB PSRAM)
- **Flash**: 16 MB Quad SPI Flash (QIO mode)
- **PSRAM**: 8 MB Octal SPI PSRAM (OPI mode)
- **USB**: Native USB-OTG support (CDC enabled)
- **Recommended for**: Large applications, web interfaces, data logging

**PlatformIO Configuration:**
```ini
[env:esp32s3_n16r8]
platform = espressif32@^6.5.0
board = esp32-s3-devkitc-1
board_build.flash_mode = qio
board_build.flash_size = 16MB
board_build.partitions = huge_app.csv
board_build.arduino.memory_type = qio_opi
board_build.psram = enabled
build_flags = 
    -D BOARD_HAS_PSRAM
    -D ARDUINO_USB_CDC_ON_BOOT=1
```

##### ESP32-S3 N8R8 (8MB Flash / 8MB PSRAM)
- **Flash**: 8 MB Quad SPI Flash
- **PSRAM**: 8 MB Quad SPI PSRAM
- **USB**: Native USB-OTG support
- **Recommended for**: Standard applications with moderate memory needs

**PlatformIO Configuration:**
```ini
[env:esp32s3_n8r8]
platform = espressif32@^6.5.0
board = esp32-s3-devkitc-1
board_build.flash_size = 8MB
board_build.partitions = huge_app.csv
board_build.psram = enabled
build_flags = 
    -D BOARD_HAS_PSRAM
```

#### Pin Configuration

ESP32-S3 has flexible GPIO multiplexing. Default pins used by ESP32-Test:

| Function | GPIO | Notes |
|----------|------|-------|
| USB D- | 19 | Native USB (if CDC enabled) |
| USB D+ | 20 | Native USB (if CDC enabled) |
| UART TX | 43 | Serial debug output |
| UART RX | 44 | Serial debug input |
| Boot | 0 | Hold LOW during reset to enter download mode |
| Reset | EN | External reset button |

**WiFi/BLE**: Internal antenna (built-in)

#### LED Indicators

- **RGB LED**: GPIO 48 (on some variants)
- **Power LED**: Always on when powered

### Freenove ESP32-S3 WROOM

Freenove's ESP32-S3 WROOM board is a compatible alternative to official DevKitC.

#### Variant: N8R8 (8MB Flash / 8MB PSRAM)
- **Flash**: 8 MB
- **PSRAM**: 8 MB
- **Layout**: Compatible with DevKitC pinout
- **USB**: USB-C connector with CH340 converter

**PlatformIO Configuration:**
```ini
[env:freenove_esp32_s3_wroom]
platform = espressif32
board = freenove_esp32_s3_wroom
framework = arduino
board_build.flash_size = 8MB
```

For 16MB variants (clones):
```ini
[env:freenove_esp32_s3_wroom_16mb]
board_build.flash_size = 16MB
board_build.flash_mode = qio
board_build.partitions = huge_app.csv
board_build.psram = enabled
```

### ESP32 Classic (DevKitC)

Original ESP32 with single or dual-core Tensilica LX6 processor.

#### Standard Configuration
- **Flash**: 4 MB (typical) or 8 MB
- **PSRAM**: Not available on standard boards
- **WiFi**: 2.4 GHz 802.11 b/g/n
- **Bluetooth**: Classic BT + BLE

**PlatformIO Configuration:**
```ini
[env:esp32devkitc]
platform = espressif32@^6.5.0
board = esp32dev
board_build.flash_size = 4MB
board_build.partitions = default.csv
```

#### Pin Configuration (DevKitC v4)

| Function | GPIO | Notes |
|----------|------|-------|
| UART TX | 1 | Serial TX (TXD0) |
| UART RX | 3 | Serial RX (RXD0) |
| Boot | 0 | Boot mode selection |
| LED | 2 | Built-in LED (active high) |
| Reset | EN | External reset |

## Power Requirements

### All Boards
- **Voltage**: 5V via USB or 3.3V via VIN pin
- **Current**: 
  - Idle: ~80 mA
  - WiFi active: 150-200 mA
  - Peak (TX): up to 500 mA
- **Recommended**: Use quality USB cable and power supply (≥500 mA)

### Power Consumption by Mode

| Mode | ESP32-S3 | ESP32 Classic |
|------|----------|---------------|
| Active (WiFi) | 80-120 mA | 80-100 mA |
| Modem-sleep | 20-30 mA | 20-25 mA |
| Light-sleep | 0.8 mA | 0.8 mA |
| Deep-sleep | 10 µA | 10 µA |

## Upload Methods

### USB CDC (ESP32-S3 only)
- Native USB support (no external converter needed)
- Enable with `ARDUINO_USB_CDC_ON_BOOT=1`
- Automatically detected on Windows/Linux/macOS

### UART via USB-to-Serial
- CH340, CP2102, FT232 converters
- Standard baud rates: 115200, 460800, 921600
- May require driver installation

### Programming via JTAG
- Use ESP-Prog or similar debugger
- Faster uploads and real-time debugging
- Configure pins: TCK, TDI, TDO, TMS

## Flash Partition Schemes

### huge_app.csv (ESP32-S3 N16R8/N8R8)
Maximizes application space for complex web interfaces:

```
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x5000
otadata,  data, ota,     0xe000,  0x2000
app0,     app,  ota_0,   0x10000, 0x300000
app1,     app,  ota_1,   0x310000,0x300000
spiffs,   data, spiffs,  0x610000,0x9F0000
```

### default.csv (ESP32 Classic)
Standard partition for simple applications:

```
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x5000
otadata,  data, ota,     0xe000,  0x2000
app0,     app,  ota_0,   0x10000, 0x140000
app1,     app,  ota_1,   0x150000,0x140000
spiffs,   data, spiffs,  0x290000,0x170000
```

## Troubleshooting

### Upload Issues

**Problem**: "Failed to connect"
- **Solution**: Hold BOOT button while clicking reset, then release
- Check USB cable quality (data cables only)
- Lower upload speed in `platformio.ini`: `upload_speed = 115200`

**Problem**: "Timed out waiting for packet header"
- **Solution**: Close serial monitor before uploading
- Disable antivirus temporarily
- Try different USB port

### Runtime Issues

**Problem**: Boot loop / constant resets
- **Solution**: Check power supply (use powered USB hub if needed)
- Verify flash size matches physical chip
- Disable PSRAM if not available: remove `BOARD_HAS_PSRAM`

**Problem**: WiFi not connecting
- **Solution**: Check `secrets.h` credentials
- Verify 2.4 GHz WiFi (5 GHz not supported)
- Move closer to router

**Problem**: Web interface not accessible
- **Solution**: Check serial monitor for IP address
- Ensure device and computer on same network
- Try IP access before mDNS (`.local`)

## Best Practices

1. **Always use quality USB cables** with data lines (not charge-only)
2. **Power from USB hub or wall adapter** for stable operation
3. **Keep firmware updated** via PlatformIO platform updates
4. **Use appropriate partition scheme** for your flash size
5. **Monitor serial output** during first upload to verify configuration
6. **Test with minimal code first** before complex applications

## Additional Resources

- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [PlatformIO ESP32 Platform](https://docs.platformio.org/en/latest/platforms/espressif32.html)
- [Arduino ESP32 Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
