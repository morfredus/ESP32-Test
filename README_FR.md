# ESP32-Test

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-compatible-blue.svg)](https://platformio.org/)
[![Version](https://img.shields.io/badge/version-0.1.1-green.svg)](CHANGELOG_FR.md)

**ESP32-Test** est un outil léger de diagnostic système basé sur le web pour microcontrôleurs ESP32. Il fournit des informations matérielles en temps réel via une interface web claire, idéal pour tester et monitorer les cartes ESP32-S3, ESP32-S2 et ESP32 Classic.

[🇬🇧 English version](README.md)

## ✨ Fonctionnalités

- 🌐 **Interface Web** : Tableau de bord clair et responsive accessible via WiFi
- 🔍 **Informations Système** : Modèle de puce, cœurs, fréquence, température
- 💾 **Surveillance Mémoire** : Utilisation du Heap et de la PSRAM avec indicateurs visuels
- 📊 **Détails Flash** : Taille totale (8/16 MB), vitesse, mode détecté
- 🗂️ **Table des Partitions** : Vue complète des partitions de la mémoire flash
- 🔌 **Infos Réseau** : SSID WiFi, IP, adresses MAC, RSSI
- 🔄 **Multi-Réseau** : Connexion automatique au meilleur WiFi disponible
- 📱 **Support mDNS** : Accès via `http://ESP32-Test.local`
- 🏷️ **Détection du Module** : Affichage automatique de la référence N8R8/N16R8

## 🖼️ Capture d’écran

L’interface web affiche toutes les informations critiques dans des cartes organisées avec mise à jour toutes les 10 secondes.

## 🛠️ Support Matériel

### Cartes Testées
- ✅ ESP32-S3-DevKitC-1 (N8R8 / N16R8)
- ✅ Freenove ESP32-S3 WROOM (N8R8)
- ✅ ESP32 Classic (DevKitC)

### Spécifications
| Modèle | Flash | PSRAM | USB CDC | Statut |
|-------|-------|-------|---------|--------|
| ESP32-S3 N16R8 | 16 MB | 8 MB | ✅ | Testé |
| ESP32-S3 N8R8 | 8 MB | 8 MB | ✅ | Testé |
| ESP32 Classic | 4-8 MB | ❌ | ❌ | Compatible |

Voir [HARDWARE_FR.md](docs/HARDWARE_FR.md) pour les configurations de broches détaillées.

## 📦 Installation

### Prérequis
- [PlatformIO](https://platformio.org/) (recommandé) ou Arduino IDE
- Carte ESP32 (voir modèles supportés ci-dessus)
- Câble USB pour le téléversement

### Démarrage Rapide

1. **Cloner le dépôt**
```bash
git clone https://github.com/yourusername/ESP32-Test.git
cd ESP32-Test
```

2. **Configurer les identifiants WiFi**

Copiez le fichier modèle et éditez vos identifiants :
```bash
# Copier le template
cp include/secrets_exemple.h include/secrets.h

# Éditer avec vos paramètres WiFi
```

Éditez `include/secrets.h` :
```cpp
const char* WIFI_SSID_1 = "your_wifi_ssid";
const char* WIFI_PASS_1 = "your_wifi_password";

// Optionnel : Ajouter un second réseau
const char* WIFI_SSID_2 = "backup_network";
const char* WIFI_PASS_2 = "backup_password";
```

> ⚠️ **Note** : `secrets.h` est ignoré par git pour protéger vos identifiants.

3. **Sélectionner l’environnement de carte**

Éditez `platformio.ini` et définissez votre carte :
```ini
[platformio]
default_envs = esp32s3_n16r8  ; ou esp32s3_n8r8, esp32devkitc
```

4. **Compiler et téléverser**
```bash
# Via PlatformIO CLI
platformio run -t upload

# Ou avec PlatformIO IDE : bouton "Upload"
```

5. **Monitorer la sortie série**
```bash
platformio device monitor -b 115200
```

6. **Accéder à l’interface web**
- Via IP : consultez l’adresse IP dans le moniteur série
- Via mDNS : `http://ESP32-Test.local`

## 🔧 Configuration

### Environnements

Le projet inclut trois environnements préconfigurés :

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

### Personnalisation

- **Nom d’hôte** : Éditer `HOSTNAME` dans `src/main.cpp`
- **Vitesse du moniteur** : Modifier `monitor_speed` dans `platformio.ini`
- **Vitesse d’upload** : Ajuster `upload_speed` si nécessaire

## 📡 Référence API

L’interface web consomme une API JSON à `/api/data` :

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

Voir [API_FR.md](docs/API_FR.md) pour la documentation complète.

## 🤝 Contribuer

Les contributions sont bienvenues ! Merci de :

1. Forker le dépôt
2. Créer une branche de fonctionnalité (`git checkout -b feature/ma-fonctionnalite`)
3. Commiter vos changements (`git commit -m 'Ajout fonctionnalité'`)
4. Pousser la branche (`git push origin feature/ma-fonctionnalite`)
5. Ouvrir une Pull Request

## 📝 Licence

Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE).

## 🙏 Remerciements

- Développé avec [PlatformIO](https://platformio.org/)
- ESP32 Arduino Core par Espressif Systems
- Interface web inspirée des tableaux de bord modernes

## 📞 Support

- 📖 [Documentation](docs/)
- 🐛 [Issues](https://github.com/yourusername/ESP32-Test/issues)
- 💬 [Discussions](https://github.com/yourusername/ESP32-Test/discussions)

## 🗺️ Feuille de Route

- [ ] Mises à jour firmware OTA
- [ ] Graphiques de données historiques
- [ ] Configuration via interface web
- [ ] Intégration MQTT
- [ ] API REST pour outils externes
- [ ] Application mobile compagnon

---

Fait avec ❤️ pour la communauté ESP32

