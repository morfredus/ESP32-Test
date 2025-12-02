#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_partition.h>
#include <esp_system.h>
#include <esp_mac.h>
#include <rom/rtc.h>

// Inclusion des identifiants WiFi
#include "secrets.h"
#include "web_interface.h"

// Configuration
const char* HOSTNAME = "ESP32-Test"; // http://ESP32-Test.local

WebServer server(80);
WiFiMulti wifiMulti;

// --- HELPER FUNCTIONS ---

String getResetReason(int cpu_no) {
    RESET_REASON reason = rtc_get_reset_reason(cpu_no);
    switch (reason) {
        case 1: return "Power on";
        case 3: return "Software";
        case 4: return "Panic/Exception";
        case 5: return "Interrupt WDT";
        case 6: return "Task WDT";
        case 7: return "Other WDT";
        case 8: return "Deep Sleep";
        case 9: return "Brownout";
        case 10: return "SDIO";
        default: return "Unknown (" + String(reason) + ")";
    }
}

String getFlashMode() {
    #if defined(CONFIG_ESPTOOLPY_FLASHMODE_QIO)
        return "QIO";
    #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_QOUT)
        return "QOUT";
    #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_DIO)
        return "DIO";
    #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_DOUT)
        return "DOUT";
    #else
        return "Unknown";
    #endif
}

// --- API DATA HANDLER ---
void handleData() {
    String json = "{";

    // 1. CHIP INFO
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    
    json += "\"chip\":{";
    json += "\"model\":\"";
    #if defined(CONFIG_IDF_TARGET_ESP32)
        json += "ESP32";
    #elif defined(CONFIG_IDF_TARGET_ESP32S2)
        json += "ESP32-S2";
    #elif defined(CONFIG_IDF_TARGET_ESP32S3)
        json += "ESP32-S3";
    #elif defined(CONFIG_IDF_TARGET_ESP32C3)
        json += "ESP32-C3";
    #else
        json += "Unknown ESP";
    #endif
    json += "\",";

    json += "\"cores\":" + String(chip_info.cores) + ",";
    json += "\"revision\":" + String(chip_info.revision) + ",";
    json += "\"freq\":" + String(getCpuFrequencyMhz()) + ",";
    
    float temp = temperatureRead();
    if (!isnan(temp)) json += "\"temp\":" + String(temp) + ",";
    else json += "\"temp\":null,";

    json += "\"features\":{";
    json += "\"wifi_bgn\":" + String((chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "true" : "false") + ",";
    json += "\"ble\":" + String((chip_info.features & CHIP_FEATURE_BLE) ? "true" : "false") + ",";
    json += "\"bt\":" + String((chip_info.features & CHIP_FEATURE_BT) ? "true" : "false");
    json += "}"; 
    json += "},";

    // 2. SYSTEM INFO
    json += "\"sys\":{";
    json += "\"sdk\":\"" + String(ESP.getSdkVersion()) + "\",";
    json += "\"idf\":\"" + String(esp_get_idf_version()) + "\",";
    json += "\"uptime\":" + String(millis()) + ",";
    json += "\"reset_reason\":\"" + getResetReason(0) + "\",";
    json += "\"app_version\":\"" + String(PROJECT_VERSION_MAJOR) + "." + String(PROJECT_VERSION_MINOR) + "." + String(PROJECT_VERSION_PATCH) + "\"";
    json += "},";

    // 3. MEMORY (RAM)
    json += "\"mem\":{";
    json += "\"heap_total\":" + String(ESP.getHeapSize()) + ",";
    json += "\"heap_free\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"heap_min_free\":" + String(ESP.getMinFreeHeap()) + ",";
    json += "\"heap_max_alloc\":" + String(ESP.getMaxAllocHeap()) + ",";
    json += "\"psram_total\":" + String(ESP.getPsramSize()) + ",";
    json += "\"psram_free\":" + String(ESP.getFreePsram()) + ",";
    json += "\"psram_max_alloc\":" + String(ESP.getMaxAllocPsram());
    json += "},";

    // 4. FLASH + RÉFÉRENCE CARTE (N8R8 / N16R8)
    uint32_t flash_size_real = 0;         // Taille physique détectée
    esp_flash_get_size(NULL, &flash_size_real);
    uint32_t flash_size_cfg = ESP.getFlashChipSize(); // Taille configurée (PlatformIO)
    float flash_mb = (float)flash_size_cfg / (1024.0f * 1024.0f);
    uint32_t psram_size = ESP.getPsramSize();

    String module_ref = "Unknown";
    if (flash_size_cfg >= 16 * 1024 * 1024UL && psram_size >= 8 * 1024 * 1024UL) {
        module_ref = "N16R8";
    } else if (flash_size_cfg >= 8 * 1024 * 1024UL && psram_size >= 8 * 1024 * 1024UL) {
        module_ref = "N8R8";
    } else if (flash_size_cfg >= 16 * 1024 * 1024UL) {
        module_ref = "N16Rx"; // 16MB flash, PSRAM inconnue ou absente
    } else if (flash_size_cfg >= 8 * 1024 * 1024UL) {
        module_ref = "N8Rx";  // 8MB flash, PSRAM inconnue ou absente
    }

    json += "\"flash\":{";
    json += "\"real_size\":" + String(flash_size_real) + ",";
    json += "\"size\":" + String(flash_size_cfg) + ",";
    json += "\"size_mb\":" + String(flash_mb, 1) + ",";
    json += "\"speed\":" + String(ESP.getFlashChipSpeed()) + ",";
    json += "\"mode\":\"" + getFlashMode() + "\"";
    json += "},";

    // 4b. MODULE REFERENCE
    json += "\"module\":{\"ref\":\"" + module_ref + "\"},";

    // 5. NETWORK
    json += "\"net\":{";
    json += "\"ssid\":\"" + WiFi.SSID() + "\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"mac\":\"" + WiFi.macAddress() + "\",";
    json += "\"bt_mac\":\"" + WiFi.macAddress() + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI());
    json += "},";

    // 6. PARTITIONS
    json += "\"partitions\":[";
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    bool first = true;
    while (it != NULL) {
        const esp_partition_t *part = esp_partition_get(it);
        if (!first) json += ",";
        json += "{";
        json += "\"label\":\"" + String(part->label) + "\",";
        
        String typeStr = (part->type == ESP_PARTITION_TYPE_APP) ? "APP" : ((part->type == ESP_PARTITION_TYPE_DATA) ? "DATA" : "OTHER");
        json += "\"type\":\"" + typeStr + "\",";
        
        String subType = String(part->subtype);
        if(part->type == ESP_PARTITION_TYPE_APP) {
             if(part->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY) subType = "FACTORY";
             else if(part->subtype >= ESP_PARTITION_SUBTYPE_APP_OTA_0 && part->subtype < ESP_PARTITION_SUBTYPE_APP_OTA_MAX) subType = "OTA_" + String(part->subtype - ESP_PARTITION_SUBTYPE_APP_OTA_0);
        } else if (part->type == ESP_PARTITION_TYPE_DATA) {
             if(part->subtype == ESP_PARTITION_SUBTYPE_DATA_OTA) subType = "OTA_DATA";
             else if(part->subtype == ESP_PARTITION_SUBTYPE_DATA_NVS) subType = "NVS";
             else if(part->subtype == ESP_PARTITION_SUBTYPE_DATA_SPIFFS) subType = "SPIFFS/LITTLEFS";
        }

        json += "\"subtype\":\"" + subType + "\",";
        json += "\"addr\":" + String(part->address) + ",";
        json += "\"size\":" + String(part->size);
        json += "}";
        first = false;
        it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it);
    json += "]";

    json += "}";
    server.send(200, "application/json", json);
}

void setup() {
    Serial.begin(115200);
    
    // Configuration WiFi via WiFiMulti
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);

    // Enregistrement des réseaux depuis secrets.h (variables WIFI_SSID_n / WIFI_PASS_n)
    int numNetworks = 0;
    if (WIFI_SSID_1 && WIFI_PASS_1 && strlen(WIFI_SSID_1) > 0) {
        wifiMulti.addAP(WIFI_SSID_1, WIFI_PASS_1);
        Serial.printf(" - Ajout reseau : %s\n", WIFI_SSID_1);
        numNetworks++;
    }
    if (WIFI_SSID_2 && WIFI_PASS_2 && strlen(WIFI_SSID_2) > 0) {
        wifiMulti.addAP(WIFI_SSID_2, WIFI_PASS_2);
        Serial.printf(" - Ajout reseau : %s\n", WIFI_SSID_2);
        numNetworks++;
    }
    Serial.printf("\nChargement de %d reseaux WiFi...\n", numNetworks);
    
    Serial.print("Recherche du meilleur reseau...");
    // Boucle d'attente de connexion (bloquant au démarrage)
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnecte!");
    Serial.print("SSID: "); Serial.println(WiFi.SSID());
    Serial.print("IP: "); Serial.println(WiFi.localIP());
    Serial.printf("Acces: http://%s.local\n", HOSTNAME);

    // Démarrage mDNS
    if (MDNS.begin(HOSTNAME)) {
        Serial.println("mDNS demarre");
    }

    // Routes Web
    server.on("/", []() {
        server.send(200, "text/html", INDEX_HTML);
    });
    server.on("/api/data", handleData);

    server.begin();
}

void loop() {
    // WiFiMulti gère automatiquement la reconnexion si le signal est perdu
    // tant qu'on appelle wifiMulti.run() régulièrement (ou qu'on laisse la stack gérer)
    // Pour une application webserver simple, vérifier la connexion ici n'est pas strictement nécessaire
    // si on accepte une coupure temporaire, mais c'est une bonne pratique.
    if (wifiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi perdu, tentative de reconnexion...");
    }

    server.handleClient();
    delay(2);
}