# Référence API

ESP32-Test expose une API REST JSON pour récupérer les diagnostics système et informations matérielles.

## Points de Terminaison

### GET `/api/data`

Retourne les informations système complètes au format JSON.

**Format de Réponse :** `application/json`

**Exemple de Requête :**
```bash
curl http://ESP32-Test.local/api/data
# ou
curl http://192.168.1.100/api/data
```

## Schéma de Réponse

### Exemple de Réponse Complète

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
    "ssid": "MonWiFi",
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

## Définitions des Objets

### chip

Informations matérielles et capacités de la puce.

| Champ | Type | Description | Unité |
|-------|------|-------------|-------|
| `model` | string | Nom du modèle de puce | - |
| `cores` | integer | Nombre de cœurs CPU | cœurs |
| `revision` | integer | Numéro de révision silicium | - |
| `freq` | integer | Fréquence CPU | MHz |
| `temp` | float/null | Capteur température interne | °C |
| `features.wifi_bgn` | boolean | Support WiFi 802.11b/g/n | - |
| `features.ble` | boolean | Support Bluetooth Low Energy | - |
| `features.bt` | boolean | Support Bluetooth Classic | - |

**Notes :**
- `temp` peut être `null` si capteur non disponible
- ESP32-S3 a typiquement BLE mais pas BT Classic
- ESP32 Classic a BLE et BT

### sys

Informations système et firmware.

| Champ | Type | Description |
|-------|------|-------------|
| `sdk` | string | Version SDK Arduino |
| `idf` | string | Version framework ESP-IDF |
| `uptime` | integer | Millisecondes depuis démarrage |
| `reset_reason` | string | Cause dernier reset |
| `app_version` | string | Version application (semver) |

**Raisons de Reset :**
- `"Power on"` - Démarrage à froid
- `"Software"` - Reset logiciel
- `"Panic/Exception"` - Crash ou exception
- `"Interrupt WDT"` - Timeout watchdog interruption
- `"Task WDT"` - Timeout watchdog tâche
- `"Deep Sleep"` - Réveil sommeil profond
- `"Brownout"` - Reset basse tension

### mem

Statistiques d'utilisation mémoire (RAM).

| Champ | Type | Description | Unité |
|-------|------|-------------|-------|
| `heap_total` | integer | Taille heap totale | octets |
| `heap_free` | integer | Heap actuellement libre | octets |
| `heap_min_free` | integer | Heap libre minimum depuis boot | octets |
| `heap_max_alloc` | integer | Plus grand bloc allouable | octets |
| `psram_total` | integer | PSRAM totale (0 si indisponible) | octets |
| `psram_free` | integer | PSRAM actuellement libre | octets |
| `psram_max_alloc` | integer | Plus grand bloc PSRAM | octets |

**Notes :**
- Champs PSRAM à 0 sur cartes sans PSRAM
- `heap_min_free` indique pression mémoire maximale
- `heap_max_alloc` montre fragmentation (plus petit = plus fragmenté)

### flash

Informations mémoire flash.

| Champ | Type | Description | Unité |
|-------|------|-------------|-------|
| `real_size` | integer | Flash physique détectée par bootloader | octets |
| `size` | integer | Taille flash configurée (platformio.ini) | octets |
| `size_mb` | float | Taille flash en mégaoctets | MB |
| `speed` | integer | Fréquence horloge flash | Hz |
| `mode` | string | Mode SPI flash | - |

**Modes Flash :**
- `"QIO"` - Quad I/O (plus rapide)
- `"QOUT"` - Quad Output
- `"DIO"` - Dual I/O
- `"DOUT"` - Dual Output

### module

Identifiant de référence module.

| Champ | Type | Description |
|-------|------|-------------|
| `ref` | string | Code référence module |

**Codes de Référence :**
- `"N16R8"` - 16MB Flash + 8MB PSRAM
- `"N8R8"` - 8MB Flash + 8MB PSRAM
- `"N16Rx"` - 16MB Flash, PSRAM inconnue
- `"N8Rx"` - 8MB Flash, PSRAM inconnue
- `"Unknown"` - Impossible à déterminer

### net

Informations réseau.

| Champ | Type | Description | Unité |
|-------|------|-------------|-------|
| `ssid` | string | Nom réseau WiFi connecté | - |
| `ip` | string | Adresse IPv4 | - |
| `mac` | string | Adresse MAC WiFi | - |
| `bt_mac` | string | Adresse MAC Bluetooth | - |
| `rssi` | integer | Force du signal | dBm |

**Interprétation RSSI :**
- `-30 à -50 dBm` - Signal excellent
- `-50 à -60 dBm` - Bon signal
- `-60 à -70 dBm` - Signal correct
- `-70 à -80 dBm` - Signal faible
- `< -80 dBm` - Signal très faible

### partitions

Tableau d'informations partitions flash.

Chaque objet partition contient :

| Champ | Type | Description | Unité |
|-------|------|-------------|-------|
| `label` | string | Nom partition | - |
| `type` | string | Type partition | - |
| `subtype` | string | Sous-type partition | - |
| `addr` | integer | Adresse départ en flash | octets |
| `size` | integer | Taille partition | octets |

**Types de Partition :**
- `"APP"` - Firmware application
- `"DATA"` - Stockage données
- `"OTHER"` - Usage spécial

**Sous-types Communs :**
- APP : `"FACTORY"`, `"OTA_0"`, `"OTA_1"`, etc.
- DATA : `"NVS"`, `"OTA_DATA"`, `"SPIFFS/LITTLEFS"`

## Exemples d'Utilisation

### Python

```python
import requests
import json

# Récupérer données
response = requests.get('http://ESP32-Test.local/api/data')
data = response.json()

# Afficher infos puce
print(f"Modèle: {data['chip']['model']}")
print(f"Cœurs: {data['chip']['cores']}")
print(f"Fréquence: {data['chip']['freq']} MHz")

# Vérifier utilisation mémoire
heap_used_pct = (1 - data['mem']['heap_free'] / data['mem']['heap_total']) * 100
print(f"Utilisation Heap: {heap_used_pct:.1f}%")

# Afficher partitions
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
    
    console.log(`Modèle: ${data.chip.model}`);
    console.log(`Version: ${data.sys.app_version}`);
    console.log(`IP: ${data.net.ip}`);
    console.log(`Flash: ${data.flash.size_mb} MB`);
    
    // Calculer uptime
    const uptime_sec = data.sys.uptime / 1000;
    console.log(`Uptime: ${uptime_sec.toFixed(0)} secondes`);
    
  } catch (error) {
    console.error('Erreur récupération données:', error);
  }
}

getESP32Info();
```

### curl

```bash
# Affichage JSON formaté
curl -s http://ESP32-Test.local/api/data | jq .

# Extraire champs spécifiques
curl -s http://ESP32-Test.local/api/data | jq '.chip.model, .flash.size_mb'

# Surveiller uptime
watch -n 5 'curl -s http://ESP32-Test.local/api/data | jq .sys.uptime'
```

## Réponses d'Erreur

L'API ne retourne actuellement pas de codes d'erreur explicites. Si le point de terminaison est indisponible :

- **HTTP 404** : Serveur ne répond pas (appareil hors ligne ou mauvaise IP)
- **Timeout** : Problème réseau ou appareil bloqué

Les versions futures pourront ajouter gestion d'erreur avec codes de statut.

## Limitation de Débit

Aucune limitation de débit actuellement implémentée. Cependant :

- Éviter polling plus rapide que 1 Hz (1 requête/seconde)
- Interface web rafraîchit automatiquement toutes les 10 secondes
- Requêtes excessives peuvent impacter performance WiFi

## CORS

Cross-Origin Resource Sharing (CORS) n'est pas actuellement configuré. Pour accès depuis applications web :

1. Servir depuis même domaine/IP que ESP32
2. Utiliser proxy CORS pour développement
3. Versions futures pourront ajouter en-têtes CORS

## Versionnage

L'API suit la version application (versionnage sémantique) :

- **0.1.x** : Structure API initiale (actuelle)
- **0.2.x** : Peut ajouter nouveaux champs (rétrocompatible)
- **1.0.0** : Contrat API stable

Vérifier `sys.app_version` pour déterminer compatibilité API.

## Améliorations Futures

Améliorations prévues pour versions futures :

- [ ] Points terminaison POST pour configuration
- [ ] WebSocket pour mises à jour temps réel
- [ ] Authentification/clés API
- [ ] Filtrage/pagination pour partitions
- [ ] Points terminaison données historiques
- [ ] Points terminaison contrôle système (redémarrage, OTA)
- [ ] Documentation JSON Schema
- [ ] Spécification OpenAPI/Swagger

---

Pour détails d'implémentation, voir code source dans `src/main.cpp` (fonction `handleData()`).
