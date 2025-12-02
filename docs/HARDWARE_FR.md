# Guide de Configuration Matérielle

## Cartes Supportées

### ESP32-S3-DevKitC-1

ESP32-S3-DevKitC-1 est la carte de développement officielle d'Espressif équipée du puissant ESP32-S3 dual-core avec support WiFi et BLE.

#### Variantes

##### ESP32-S3 N16R8 (16MB Flash / 8MB PSRAM)
- **Flash** : 16 MB Quad SPI Flash (mode QIO)
- **PSRAM** : 8 MB Octal SPI PSRAM (mode OPI)
- **USB** : Support USB-OTG natif (CDC activé)
- **Recommandé pour** : Applications volumineuses, interfaces web, enregistrement de données

**Configuration PlatformIO :**
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
- **Flash** : 8 MB Quad SPI Flash
- **PSRAM** : 8 MB Quad SPI PSRAM
- **USB** : Support USB-OTG natif
- **Recommandé pour** : Applications standards avec besoins mémoire modérés

**Configuration PlatformIO :**
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

#### Configuration des Broches

ESP32-S3 a un multiplexage GPIO flexible. Broches par défaut utilisées par ESP32-Test :

| Fonction | GPIO | Notes |
|----------|------|-------|
| USB D- | 19 | USB natif (si CDC activé) |
| USB D+ | 20 | USB natif (si CDC activé) |
| UART TX | 43 | Sortie debug série |
| UART RX | 44 | Entrée debug série |
| Boot | 0 | Maintenir BAS pendant reset pour mode téléchargement |
| Reset | EN | Bouton reset externe |

**WiFi/BLE** : Antenne interne (intégrée)

#### Indicateurs LED

- **LED RGB** : GPIO 48 (sur certaines variantes)
- **LED Alimentation** : Toujours allumée quand alimentée

### Freenove ESP32-S3 WROOM

La carte Freenove ESP32-S3 WROOM est une alternative compatible à la DevKitC officielle.

#### Variante : N8R8 (8MB Flash / 8MB PSRAM)
- **Flash** : 8 MB
- **PSRAM** : 8 MB
- **Layout** : Compatible avec le pinout DevKitC
- **USB** : Connecteur USB-C avec convertisseur CH340

**Configuration PlatformIO :**
```ini
[env:freenove_esp32_s3_wroom]
platform = espressif32
board = freenove_esp32_s3_wroom
framework = arduino
board_build.flash_size = 8MB
```

Pour variantes 16MB (clones) :
```ini
[env:freenove_esp32_s3_wroom_16mb]
board_build.flash_size = 16MB
board_build.flash_mode = qio
board_build.partitions = huge_app.csv
board_build.psram = enabled
```

### ESP32 Classic (DevKitC)

ESP32 original avec processeur Tensilica LX6 simple ou dual-core.

#### Configuration Standard
- **Flash** : 4 MB (typique) ou 8 MB
- **PSRAM** : Non disponible sur cartes standard
- **WiFi** : 2.4 GHz 802.11 b/g/n
- **Bluetooth** : BT Classic + BLE

**Configuration PlatformIO :**
```ini
[env:esp32devkitc]
platform = espressif32@^6.5.0
board = esp32dev
board_build.flash_size = 4MB
board_build.partitions = default.csv
```

#### Configuration des Broches (DevKitC v4)

| Fonction | GPIO | Notes |
|----------|------|-------|
| UART TX | 1 | TX série (TXD0) |
| UART RX | 3 | RX série (RXD0) |
| Boot | 0 | Sélection mode boot |
| LED | 2 | LED intégrée (actif haut) |
| Reset | EN | Reset externe |

## Besoins en Alimentation

### Toutes les Cartes
- **Tension** : 5V via USB ou 3.3V via broche VIN
- **Courant** : 
  - Repos : ~80 mA
  - WiFi actif : 150-200 mA
  - Pic (TX) : jusqu'à 500 mA
- **Recommandé** : Utiliser câble USB et alimentation de qualité (≥500 mA)

### Consommation par Mode

| Mode | ESP32-S3 | ESP32 Classic |
|------|----------|---------------|
| Actif (WiFi) | 80-120 mA | 80-100 mA |
| Modem-sleep | 20-30 mA | 20-25 mA |
| Light-sleep | 0.8 mA | 0.8 mA |
| Deep-sleep | 10 µA | 10 µA |

## Méthodes de Téléversement

### USB CDC (ESP32-S3 uniquement)
- Support USB natif (pas de convertisseur externe nécessaire)
- Activer avec `ARDUINO_USB_CDC_ON_BOOT=1`
- Détecté automatiquement sur Windows/Linux/macOS

### UART via USB-vers-Série
- Convertisseurs CH340, CP2102, FT232
- Débits standard : 115200, 460800, 921600
- Peut nécessiter installation de pilote

### Programmation via JTAG
- Utiliser ESP-Prog ou débogueur similaire
- Uploads plus rapides et débogage temps réel
- Configurer broches : TCK, TDI, TDO, TMS

## Schémas de Partitions Flash

### huge_app.csv (ESP32-S3 N16R8/N8R8)
Maximise l'espace application pour interfaces web complexes :

```
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x5000
otadata,  data, ota,     0xe000,  0x2000
app0,     app,  ota_0,   0x10000, 0x300000
app1,     app,  ota_1,   0x310000,0x300000
spiffs,   data, spiffs,  0x610000,0x9F0000
```

### default.csv (ESP32 Classic)
Partition standard pour applications simples :

```
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x5000
otadata,  data, ota,     0xe000,  0x2000
app0,     app,  ota_0,   0x10000, 0x140000
app1,     app,  ota_1,   0x150000,0x140000
spiffs,   data, spiffs,  0x290000,0x170000
```

## Dépannage

### Problèmes de Téléversement

**Problème** : "Failed to connect"
- **Solution** : Maintenir bouton BOOT en cliquant sur reset, puis relâcher
- Vérifier qualité câble USB (câbles données uniquement)
- Baisser vitesse upload dans `platformio.ini` : `upload_speed = 115200`

**Problème** : "Timed out waiting for packet header"
- **Solution** : Fermer moniteur série avant téléversement
- Désactiver antivirus temporairement
- Essayer port USB différent

### Problèmes d'Exécution

**Problème** : Boucle de boot / resets constants
- **Solution** : Vérifier alimentation (utiliser hub USB alimenté si nécessaire)
- Vérifier taille flash correspond à la puce physique
- Désactiver PSRAM si non disponible : retirer `BOARD_HAS_PSRAM`

**Problème** : WiFi ne se connecte pas
- **Solution** : Vérifier identifiants dans `secrets.h`
- Vérifier WiFi 2.4 GHz (5 GHz non supporté)
- Se rapprocher du routeur

**Problème** : Interface web inaccessible
- **Solution** : Vérifier adresse IP dans moniteur série
- S'assurer appareil et ordinateur sur même réseau
- Essayer accès IP avant mDNS (`.local`)

## Bonnes Pratiques

1. **Toujours utiliser câbles USB de qualité** avec lignes données (pas charge seule)
2. **Alimenter depuis hub USB ou adaptateur mural** pour fonctionnement stable
3. **Garder firmware à jour** via mises à jour plateforme PlatformIO
4. **Utiliser schéma partition approprié** pour votre taille flash
5. **Surveiller sortie série** pendant premier upload pour vérifier configuration
6. **Tester avec code minimal d'abord** avant applications complexes

## Ressources Additionnelles

- [Datasheet ESP32-S3](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [Datasheet ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [Plateforme PlatformIO ESP32](https://docs.platformio.org/en/latest/platforms/espressif32.html)
- [Documentation Arduino ESP32 Core](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
