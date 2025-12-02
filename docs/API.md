# API Reference

ESP32-Test exposes a JSON REST API for retrieving system diagnostics and hardware information.

## Endpoints

### GET `/api/data`

Returns complete system information in JSON format.

**Response Format:** `application/json`

**Example Request:**
```bash
curl http://ESP32-Test.local/api/data
# or
curl http://192.168.1.100/api/data
```

## Response Schema

### Complete Response Example

```json
{
  "chip": {
    "model": "ESP32-S3",
    "cores": 2,
    "revision": 0,
    "freq": 240,
    "temp": 45.2,
    "features": {
      "wifi_bgn": true,
      "ble": true,
      "bt": false
    }
  },
  "sys": {
    "sdk": "v3.3.4",
    "idf": "v5.1.2",
    "uptime": 125430,
    "reset_reason": "Power on",
    "app_version": "0.1.0"
  },
  "mem": {
    "heap_total": 327680,
    "heap_free": 280672,
    "heap_min_free": 275104,
    "heap_max_alloc": 245760,
    "psram_total": 8388608,
    "psram_free": 8388608,
    "psram_max_alloc": 8384512
  },
  "flash": {
    "real_size": 16777216,
    "size": 16777216,
    "size_mb": 16.0,
    "speed": 80000000,
    "mode": "QIO"
  },
  "module": {
    "ref": "N16R8"
  },
  "net": {
    "ssid": "MyWiFi",
    "ip": "192.168.1.100",
    "mac": "AA:BB:CC:DD:EE:FF",
    "bt_mac": "AA:BB:CC:DD:EE:FF",
    "rssi": -45
  },
  "partitions": [
    {
      "label": "nvs",
      "type": "DATA",
      "subtype": "NVS",
      "addr": 36864,
      "size": 20480
    },
    {
      "label": "otadata",
      "type": "DATA",
      "subtype": "OTA_DATA",
      "addr": 57344,
      "size": 8192
    },
    {
      "label": "app0",
      "type": "APP",
      "subtype": "OTA_0",
      "addr": 65536,
      "size": 3145728
    }
  ]
}
```

## Object Definitions

### chip

Chip hardware information and capabilities.

| Field | Type | Description | Unit |
|-------|------|-------------|------|
| `model` | string | Chip model name | - |
| `cores` | integer | Number of CPU cores | cores |
| `revision` | integer | Silicon revision number | - |
| `freq` | integer | CPU frequency | MHz |
| `temp` | float/null | Internal temperature sensor | °C |
| `features.wifi_bgn` | boolean | WiFi 802.11b/g/n support | - |
| `features.ble` | boolean | Bluetooth Low Energy support | - |
| `features.bt` | boolean | Classic Bluetooth support | - |

**Notes:**
- `temp` may be `null` if temperature sensor unavailable
- ESP32-S3 typically has BLE but not Classic BT
- ESP32 Classic has both BLE and BT

### sys

System and firmware information.

| Field | Type | Description |
|-------|------|-------------|
| `sdk` | string | Arduino SDK version |
| `idf` | string | ESP-IDF framework version |
| `uptime` | integer | Milliseconds since boot |
| `reset_reason` | string | Last reset cause |
| `app_version` | string | Application version (semver) |

**Reset Reasons:**
- `"Power on"` - Cold boot
- `"Software"` - Software reset
- `"Panic/Exception"` - Crash or exception
- `"Interrupt WDT"` - Interrupt watchdog timeout
- `"Task WDT"` - Task watchdog timeout
- `"Deep Sleep"` - Wake from deep sleep
- `"Brownout"` - Low voltage reset

### mem

Memory usage statistics (RAM).

| Field | Type | Description | Unit |
|-------|------|-------------|------|
| `heap_total` | integer | Total heap size | bytes |
| `heap_free` | integer | Currently free heap | bytes |
| `heap_min_free` | integer | Minimum free heap since boot | bytes |
| `heap_max_alloc` | integer | Largest allocatable block | bytes |
| `psram_total` | integer | Total PSRAM (0 if unavailable) | bytes |
| `psram_free` | integer | Currently free PSRAM | bytes |
| `psram_max_alloc` | integer | Largest PSRAM block | bytes |

**Notes:**
- PSRAM fields are 0 on boards without PSRAM
- `heap_min_free` indicates worst-case memory pressure
- `heap_max_alloc` shows fragmentation (smaller = more fragmented)

### flash

Flash memory information.

| Field | Type | Description | Unit |
|-------|------|-------------|------|
| `real_size` | integer | Physical flash detected by bootloader | bytes |
| `size` | integer | Configured flash size (platformio.ini) | bytes |
| `size_mb` | float | Flash size in megabytes | MB |
| `speed` | integer | Flash clock frequency | Hz |
| `mode` | string | Flash SPI mode | - |

**Flash Modes:**
- `"QIO"` - Quad I/O (fastest)
- `"QOUT"` - Quad Output
- `"DIO"` - Dual I/O
- `"DOUT"` - Dual Output

### module

Module reference identifier.

| Field | Type | Description |
|-------|------|-------------|
| `ref` | string | Module reference code |

**Reference Codes:**
- `"N16R8"` - 16MB Flash + 8MB PSRAM
- `"N8R8"` - 8MB Flash + 8MB PSRAM
- `"N16Rx"` - 16MB Flash, PSRAM unknown
- `"N8Rx"` - 8MB Flash, PSRAM unknown
- `"Unknown"` - Cannot determine

### net

Network information.

| Field | Type | Description | Unit |
|-------|------|-------------|------|
| `ssid` | string | Connected WiFi network name | - |
| `ip` | string | IPv4 address | - |
| `mac` | string | WiFi MAC address | - |
| `bt_mac` | string | Bluetooth MAC address | - |
| `rssi` | integer | Signal strength | dBm |

**RSSI Interpretation:**
- `-30 to -50 dBm` - Excellent signal
- `-50 to -60 dBm` - Good signal
- `-60 to -70 dBm` - Fair signal
- `-70 to -80 dBm` - Weak signal
- `< -80 dBm` - Very weak signal

### partitions

Array of flash partition information.

Each partition object contains:

| Field | Type | Description | Unit |
|-------|------|-------------|------|
| `label` | string | Partition name | - |
| `type` | string | Partition type | - |
| `subtype` | string | Partition subtype | - |
| `addr` | integer | Start address in flash | bytes |
| `size` | integer | Partition size | bytes |

**Partition Types:**
- `"APP"` - Application firmware
- `"DATA"` - Data storage
- `"OTHER"` - Special purpose

**Common Subtypes:**
- APP: `"FACTORY"`, `"OTA_0"`, `"OTA_1"`, etc.
- DATA: `"NVS"`, `"OTA_DATA"`, `"SPIFFS/LITTLEFS"`

## Usage Examples

### Python

```python
import requests
import json

# Fetch data
response = requests.get('http://ESP32-Test.local/api/data')
data = response.json()

# Display chip info
print(f"Model: {data['chip']['model']}")
print(f"Cores: {data['chip']['cores']}")
print(f"Frequency: {data['chip']['freq']} MHz")

# Check memory usage
heap_used_pct = (1 - data['mem']['heap_free'] / data['mem']['heap_total']) * 100
print(f"Heap usage: {heap_used_pct:.1f}%")

# Show partitions
for part in data['partitions']:
    size_mb = part['size'] / (1024 * 1024)
    print(f"{part['label']}: {size_mb:.2f} MB ({part['type']})")
```

### JavaScript (Node.js)

```javascript
const axios = require('axios');

async function getESP32Info() {
  try {
    const response = await axios.get('http://ESP32-Test.local/api/data');
    const data = response.data;
    
    console.log(`Model: ${data.chip.model}`);
    console.log(`Version: ${data.sys.app_version}`);
    console.log(`IP: ${data.net.ip}`);
    console.log(`Flash: ${data.flash.size_mb} MB`);
    
    // Calculate uptime
    const uptime_sec = data.sys.uptime / 1000;
    console.log(`Uptime: ${uptime_sec.toFixed(0)} seconds`);
    
  } catch (error) {
    console.error('Error fetching data:', error);
  }
}

getESP32Info();
```

### curl

```bash
# Pretty print JSON
curl -s http://ESP32-Test.local/api/data | jq .

# Extract specific fields
curl -s http://ESP32-Test.local/api/data | jq '.chip.model, .flash.size_mb'

# Monitor uptime
watch -n 5 'curl -s http://ESP32-Test.local/api/data | jq .sys.uptime'
```

## Error Responses

The API currently does not return explicit error codes. If the endpoint is unavailable:

- **HTTP 404**: Server not responding (device offline or wrong IP)
- **Timeout**: Network issue or device hung

Future versions may add proper error handling with status codes.

## Rate Limiting

No rate limiting is currently implemented. However:

- Avoid polling faster than 1 Hz (1 request/second)
- Web interface auto-refreshes every 10 seconds
- Excessive requests may impact WiFi performance

## CORS

Cross-Origin Resource Sharing (CORS) is not currently configured. To access from web applications:

1. Serve from same domain/IP as ESP32
2. Use a CORS proxy for development
3. Future versions may add CORS headers

## Versioning

API follows the application version (semantic versioning):

- **0.1.x**: Initial API structure (current)
- **0.2.x**: May add new fields (backward compatible)
- **1.0.0**: Stable API contract

Check `sys.app_version` to determine API compatibility.

## Future Enhancements

Planned improvements for future versions:

- [ ] POST endpoints for configuration
- [ ] WebSocket for real-time updates
- [ ] Authentication/API keys
- [ ] Filtering/pagination for partitions
- [ ] Historical data endpoints
- [ ] System control endpoints (restart, OTA)
- [ ] JSON Schema documentation
- [ ] OpenAPI/Swagger specification

---

For implementation details, see the source code in `src/main.cpp` (`handleData()` function).
