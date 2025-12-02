# Journal des Modifications

Tous les changements notables de ce projet seront documentés dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/),
et ce projet adhère au [Versionnage Sémantique](https://semver.org/lang/fr/).

## [0.1.1] - 2025-12-03

### Ajouté
- Fichier template `secrets_exemple.h` pour les identifiants WiFi
- Instructions dans README pour copier le template en `secrets.h`

### Modifié
- Amélioration du processus de configuration avec template d'identifiants
- Documentation enrichie avec notes de sécurité

## [0.1.0] - 2025-12-03

### Ajouté
- Version initiale d'ESP32-Test
- Interface de diagnostic système basée sur le web
- Affichage d'informations matérielles en temps réel
- Support pour ESP32-S3 (N8R8 / N16R8) et ESP32 Classic
- Affichage des informations de la puce (modèle, cœurs, fréquence, température)
- Surveillance de la mémoire (Heap et PSRAM avec barres d'utilisation)
- Détails de la mémoire Flash (taille en MB, vitesse, mode)
- Détection automatique du module (référence N8R8/N16R8)
- Visualiseur complet de la table des partitions
- Informations réseau (SSID WiFi, IP, MAC, RSSI)
- Support multi-réseaux avec WiFiMulti
- Support mDNS pour l'accès via `ESP32-Test.local`
- Point de terminaison API JSON à `/api/data`
- Interface web responsive avec rafraîchissement automatique (10s)
- Trois environnements PlatformIO pré-configurés :
  - `esp32s3_n16r8` : ESP32-S3 avec 16MB Flash / 8MB PSRAM
  - `esp32s3_n8r8` : ESP32-S3 avec 8MB Flash / 8MB PSRAM
  - `esp32devkitc` : ESP32 Classic avec 4-8MB Flash
- Affichage du versionnage sémantique dans l'interface web
- Documentation complète (EN/FR)

### Détails Techniques
- Plateforme : ESP32 Arduino Core (ESP-IDF v5.1.2+)
- Framework : Arduino
- Système de build : PlatformIO
- Serveur web : ESPAsyncWebServer
- Mémoire : Support Heap + PSRAM
- Flash : Détection mode QIO/DIO
- Support USB CDC pour ESP32-S3

### Documentation
- README.md (Anglais)
- README_FR.md (Français)
- CHANGELOG.md (Anglais)
- CHANGELOG_FR.md (Français)
- Documentation de référence API
- Guide de configuration matérielle
- Instructions d'installation et de configuration

## [Non Publié]

### Fonctionnalités Prévues
- Capacité de mise à jour firmware OTA
- Enregistrement et graphiques de données historiques
- Interface de configuration via web
- Intégration MQTT pour plateformes IoT
- API REST étendue pour outils externes
- Application mobile companion
- Support d'interface multi-langues
- Thèmes personnalisés et mode sombre
- Export de rapports de diagnostic (JSON/CSV)
- Diagnostics réseau avancés

---

[0.1.1]: https://github.com/yourusername/ESP32-Test/releases/tag/v0.1.1
[0.1.0]: https://github.com/yourusername/ESP32-Test/releases/tag/v0.1.0

## [0.1.2] - 2025-12-03

### Ajouté
- Script d’aide `scripts/auto_env.py` pour sélectionner automatiquement l’environnement PlatformIO selon la carte détectée

### Modifié
- README et README_FR mis à jour pour documenter l’utilisation de l’auto-détection
- Note ajoutée dans `platformio.ini` pour l’assistant

### Documentation
- Harmonisation des liens et parité FR/EN

