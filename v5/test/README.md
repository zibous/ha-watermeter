# ESP32-Devkit-V4 Wasserzähler

**Gerät:** ESP32 DevKit V4 + CC1101  
**Protokoll:** wM-Bus (Wireless M-Bus) 868.95 MHz  
**Zähler:** Diehl IZAR RC 868 I R4 PL  
**Hostname:** `water-meter-esp.local`  
**IP:** `10.1.1.71`

---

## WLAN konfigurieren


### Neues WLAN hinzufügen

In `secrets.yaml` einen neuen Eintrag ergänzen:
```yaml
ssid_neu_name: "MeinNetzwerk"
ssid_neu_pswd: "MeinPasswort"
```

Dann in `esp32-devkit-v4-wasserzahler.yaml` unter `wifi: networks:` eintragen:
```yaml
- ssid: !secret ssid_neu_name
  password: !secret ssid_neu_pswd
  priority: 20
```

Danach OTA flashen:
```bash
esphome run esp32-devkit-v4-wasserzahler.yaml
```

---

## Kein WLAN verfügbar — Hotspot Modus

Wenn kein bekanntes WLAN erreichbar ist, startet das Gerät nach ca. **60 Sekunden** automatisch einen eigenen Hotspot:

| | |
|---|---|
| **SSID** | `water-meter-esp_AP` |
| **Passwort** | `055787478310` |
| **IP** | `192.168.4.1` |

### Verbindung herstellen
1. Mit Handy oder Laptop mit `water-meter-esp_AP` verbinden
2. Browser öffnet sich automatisch (Captive Portal)  
   — oder manuell: `http://192.168.4.1` aufrufen
3. Webserver Dashboard wird angezeigt

---

## Wasserzähler ID ändern

### Wann nötig?
- Bei Zählertausch durch den Wasserversorger
- Bei Umzug des Geräts zu einem anderen Zähler

### ID herausfinden
Die Zähler-ID steht auf dem Zähler (8-stellige HEX-Zahl, z.B. `24058612`).  
Alternativ alle Zähler in Reichweite empfangen:

```bash
# Wildcard setzen — alle Zähler werden im Log angezeigt
curl -X POST "http://water-meter-esp.local/text/wasseruhr_meter_id_eingabe/set" --data "value=00000000"
```
Dann im Log nach `[wmbus_scan]` Zeilen suchen:
```
Frame: RSSI=-78dBm Mode=T1 Data=1944a511 78071286 0524...
                                          ^^^^^^^^
                                          Das ist die Meter-ID (bytes 4-7, little-endian)
```

### Methode 1: curl (empfohlen, kein HA nötig)

```bash
# ID setzen (Beispiel: 24058612)
curl -X POST "http://water-meter-esp.local/text/wasseruhr_meter_id_eingabe/set" --data "value=24058612"

# Aktuelle ID prüfen
curl "http://water-meter-esp.local/text_sensor/wasseruhr_meter_id"
```

Der ESP startet automatisch nach 2 Sekunden neu und verwendet die neue ID.

### Methode 2: Webserver Dashboard

1. `http://water-meter-esp.local` im Browser öffnen
2. Feld **"Wasseruhr Meter ID Eingabe"** finden
3. 8-stellige HEX-ID eingeben (z.B. `24058612`)
4. Enter → ESP startet neu

### Methode 3: Home Assistant Service

```yaml
service: esphome.water_meter_esp_set_meter_id
data:
  new_meter_id: "24058612"
```

### Im Hotspot-Modus (kein WLAN)

```bash
curl -X POST "http://192.168.4.1/text/wasseruhr_meter_id_eingabe/set" --data "value=24058612"
```

---

## Bekannte Zähler-IDs

| ID | Beschreibung |
|---|---|
| `24058612` | Aktueller Zähler (seit 2024) |
| `43430778` | Vorheriger Zähler |
| `86120778` | Weiterer alter Zähler |
| `00000000` | Wildcard — alle Zähler empfangen |

---

## Aktuelle Werte abfragen (curl)

```bash
# Wasserstand gesamt
curl "http://water-meter-esp.local/sensor/wasseruhr_anzeige"

# Aktuelle Meter-ID
curl "http://water-meter-esp.local/text_sensor/wasseruhr_meter_id"

# Alle Sensoren (JSON)
curl "http://water-meter-esp.local/sensor/wasser_tag"
curl "http://water-meter-esp.local/sensor/wasser_stunde"
curl "http://water-meter-esp.local/sensor/wasser_gestern"
curl "http://water-meter-esp.local/sensor/wasser_monat"
curl "http://water-meter-esp.local/sensor/wasser_jahr"
```

---

## OTA Update

```bash
# Firmware neu kompilieren und flashen
esphome run esp32-devkit-v4-wasserzahler.yaml

# Nur kompilieren
esphome compile esp32-devkit-v4-wasserzahler.yaml
```

---

## Webserver

| URL | Beschreibung |
|---|---|
| `http://water-meter-esp.local` | Dashboard |
| `http://water-meter-esp.local/logs` | Live Log |
| `http://10.1.1.71` | Direkt per IP |
