#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32-Test | Rapport Système</title>
    <style>
        :root { --primary: #2563eb; --bg: #f1f5f9; --card: #ffffff; --text: #1e293b; --label: #64748b; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background: var(--bg); color: var(--text); margin: 0; padding: 20px; }
        .container { max-width: 1000px; margin: 0 auto; }
        header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; }
        h1 { margin: 0; font-size: 1.5rem; color: var(--primary); }
        .refresh-btn { background: var(--primary); color: white; border: none; padding: 8px 16px; border-radius: 6px; cursor: pointer; font-weight: 600; }
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; }
        .card { background: var(--card); padding: 20px; border-radius: 12px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
        .card h2 { margin-top: 0; border-bottom: 2px solid #e2e8f0; padding-bottom: 10px; font-size: 1.1rem; }
        .item { display: flex; justify-content: space-between; margin-bottom: 8px; border-bottom: 1px dashed #f1f5f9; padding-bottom: 4px; }
        .label { color: var(--label); font-size: 0.9rem; }
        .value { font-weight: 500; font-family: 'Consolas', monospace; }
        .bar-bg { background: #e2e8f0; height: 8px; border-radius: 4px; margin-top: 5px; overflow: hidden; }
        .bar-fill { height: 100%; background: var(--primary); transition: width 0.3s; }
        .part-table { width: 100%; border-collapse: collapse; font-size: 0.85rem; }
        .part-table th { text-align: left; color: var(--label); border-bottom: 1px solid #cbd5e1; }
        .part-table td { padding: 4px 0; border-bottom: 1px solid #f1f5f9; }
        .badge { padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; background: #e0e7ff; color: #3730a3; }
        footer { margin-top: 30px; text-align: center; font-size: 0.8rem; color: var(--label); }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>⚡ ESP32-Test <span id="app-version" style="font-size:0.8em;color:#64748b">v?.?.?</span></h1>
            <button class="refresh-btn" onclick="fetchData()">Actualiser</button>
        </header>
        
        <div id="loading">Chargement des données...</div>
        
        <div class="grid" id="dashboard" style="display:none;">
            <div class="card">
                <h2>🆔 Identité Puce</h2>
                <div id="chip-info"></div>
            </div>

            <div class="card">
                <h2>⚙️ Système & OS</h2>
                <div id="sys-info"></div>
            </div>

            <div class="card">
                <h2>🧠 Mémoire RAM (Heap + PSRAM)</h2>
                <div id="ram-info"></div>
            </div>

            <div class="card">
                <h2>💾 Stockage Flash</h2>
                <div id="flash-info"></div>
            </div>

            <div class="card" style="grid-column: 1 / -1;">
                <h2>🗂️ Table des Partitions</h2>
                <table class="part-table">
                    <thead><tr><th>Nom</th><th>Type</th><th>Sous-type</th><th>Adresse</th><th>Taille</th></tr></thead>
                    <tbody id="part-table-body"></tbody>
                </table>
            </div>
        </div>
        
        <footer>ESP32-Test Lite • Généré par le microcontrôleur</footer>
    </div>

    <script>
        function fmtBytes(bytes, decimals = 2) {
            if (!+bytes) return '0 Bytes';
            const k = 1024;
            const sizes = ['Bytes', 'KB', 'MB', 'GB'];
            const i = Math.floor(Math.log(bytes) / Math.log(k));
            return `${parseFloat((bytes / Math.pow(k, i)).toFixed(decimals))} ${sizes[i]}`;
        }

        function createItem(label, value, isBadge = false) {
            return `<div class="item"><span class="label">${label}</span><span class="value ${isBadge ? 'badge' : ''}">${value}</span></div>`;
        }

        function createBar(label, current, total) {
            const pct = ((current / total) * 100).toFixed(1);
            return `
                <div style="margin-top:12px;">
                    <div style="display:flex;justify-content:space-between;font-size:0.85rem;">
                        <span>${label}</span>
                        <span>${fmtBytes(current)} / ${fmtBytes(total)} (${pct}%)</span>
                    </div>
                    <div class="bar-bg"><div class="bar-fill" style="width:${pct}%"></div></div>
                </div>
            `;
        }

        async function fetchData() {
            try {
                const res = await fetch('/api/data');
                const data = await res.json();
                
                // Chip Info
                let chipHtml = createItem('Modèle', data.chip.model);
                chipHtml += createItem('Cœurs', data.chip.cores);
                chipHtml += createItem('Révision', `v${data.chip.revision}`);
                chipHtml += createItem('Fréquence CPU', `${data.chip.freq} MHz`);
                chipHtml += createItem('Adresse MAC (WiFi)', data.net.mac);
                chipHtml += createItem('Adresse MAC (BT)', data.net.bt_mac);
                chipHtml += createItem('Capteurs T°', data.chip.temp ? `${data.chip.temp.toFixed(1)} °C` : 'N/A');
                document.getElementById('chip-info').innerHTML = chipHtml;

                // Sys Info
                let sysHtml = createItem('SDK Version', data.sys.sdk);
                sysHtml += createItem('ESP-IDF', data.sys.idf);
                sysHtml += createItem('Version App', data.sys.app_version, true);
                sysHtml += createItem('Uptime', `${(data.sys.uptime / 1000).toFixed(0)} s`);
                sysHtml += createItem('Reset Reason', data.sys.reset_reason);
                sysHtml += createItem('IP Adresse', data.net.ip, true);
                sysHtml += createItem('WiFi SSID', data.net.ssid);
                sysHtml += createItem('RSSI', `${data.net.rssi} dBm`);
                document.getElementById('sys-info').innerHTML = sysHtml;
                document.getElementById('app-version').textContent = `v${data.sys.app_version}`;

                // RAM
                let ramHtml = createItem('Heap Total', fmtBytes(data.mem.heap_total));
                ramHtml += createItem('Heap Libre', fmtBytes(data.mem.heap_free));
                ramHtml += createItem('Max Alloc.', fmtBytes(data.mem.heap_max_alloc));
                ramHtml += createBar('Utilisation Heap', data.mem.heap_total - data.mem.heap_free, data.mem.heap_total);
                
                if(data.mem.psram_total > 0) {
                    ramHtml += `<hr style="border:0;border-top:1px solid #eee;margin:10px 0;">`;
                    ramHtml += createItem('PSRAM Total', fmtBytes(data.mem.psram_total));
                    ramHtml += createBar('Utilisation PSRAM', data.mem.psram_total - data.mem.psram_free, data.mem.psram_total);
                } else {
                    ramHtml += `<div style="margin-top:10px;color:orange;font-size:0.8rem;">⚠️ Pas de PSRAM détectée</div>`;
                }
                document.getElementById('ram-info').innerHTML = ramHtml;

                // Flash
                let flashHtml = createItem('Taille Totale', `${data.flash.size_mb} MB`);
                flashHtml += createItem('Taille (octets)', fmtBytes(data.flash.size));
                flashHtml += createItem('Vitesse', `${data.flash.speed / 1000000} MHz`);
                flashHtml += createItem('Mode', data.flash.mode);
                if (data.module && data.module.ref) {
                    flashHtml += createItem('Module', data.module.ref, true);
                }
                document.getElementById('flash-info').innerHTML = flashHtml;

                // Partitions
                let partHtml = '';
                data.partitions.forEach(p => {
                    partHtml += `<tr>
                        <td><b>${p.label}</b></td>
                        <td>${p.type}</td>
                        <td>${p.subtype}</td>
                        <td>0x${p.addr.toString(16).toUpperCase()}</td>
                        <td>${fmtBytes(p.size)}</td>
                    </tr>`;
                });
                document.getElementById('part-table-body').innerHTML = partHtml;

                document.getElementById('loading').style.display = 'none';
                document.getElementById('dashboard').style.display = 'grid';

            } catch (e) {
                document.getElementById('loading').textContent = 'Erreur de connexion : ' + e;
            }
        }

        // Chargement initial
        fetchData();
        // Rafraichissement auto lent (10s)
        setInterval(fetchData, 10000);
    </script>
</body>
</html>
)rawliteral";

#endif