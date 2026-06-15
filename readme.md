# ESP32-C3 Mini — BME280 Wetterstation mit OLED

Kompakte Wetterstation auf Basis des ESP32-C3 Super Mini Boards mit 
onboard 0.42" OLED Display und BME280 Umgebungssensor.

## Hardware

| Komponente | Details |
|---|---|
| **Board** | ESP32-C3 Super Mini mit 0.42" OLED onboard |
| **Sensor** | BME280 — Temperatur, Luftfeuchtigkeit, Luftdruck |
| **Display** | 0.42" OLED SSD1306-kompatibel, 72×40 Pixel |
| **Kommunikation** | I2C (GPIO5=SDA, GPIO6=SCL) |

## Verkabelung

BME280 und OLED teilen denselben I2C Bus (GPIO5/GPIO6):

BME280 VCC → 3.3V

BME280 GND → GND

BME280 SDA → GPIO5

BME280 SCL → GPIO6


## Software

- **Framework:** Arduino (PlatformIO)
- **Board:** esp32-c3-devkitm-1
- **Libraries:**
  - U8g2 (OLED Display)
  - Adafruit BME280
  - Adafruit Unified Sensor

## Wichtige Hinweise

- OLED I2C Adresse: `0x3C`
- BME280 I2C Adresse: `0x76`
- `Wire.begin(5, 6)` muss **nach** `u8g2.begin()` aufgerufen werden!
- USB CDC muss in platformio.ini aktiviert sein:
```ini
build_flags =
    -D ARDUINO_USB_MODE=1
    -D ARDUINO_USB_CDC_ON_BOOT=1
```

## Anzeige

Das OLED zeigt live:
- 🌡️ Temperatur in °C
- 💧 Luftfeuchtigkeit in %
- 🔵 Luftdruck in hPa

## Geplante Erweiterungen

- BLE Übertragung der Sensordaten ans Waveshare Dashboard
- NTP Uhrzeit auf dem Display
- Mehrere Sensoren im BLE Netzwerk
- Außeneinsatz mit Batteriebetrieb

## Projektkontext

Teil eines größeren ESP32 Sensor-Netzwerks:
- **Waveshare ESP32-S3 4.3"** — Zentrales Dashboard
- **ESP32-C3 Mini** — Außensensor (Balkon)
- **ESP32-C3 Mini** — Innensensoren (Schlafzimmer, Küche)