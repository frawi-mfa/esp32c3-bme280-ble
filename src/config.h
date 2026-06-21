#pragma once

// ── Sensor Typ ───────────────────────────────────────────
// #define SENSOR_BME280   // Temperatur + Feuchte + Druck
#define SENSOR_BMP280      // Nur Temperatur + Druck

// ── BLE Name (jeder Sensor braucht einen eigenen!) ───────
// #define BLE_NAME "ESP32-C3 Wetterstation"
// #define BLE_NAME "ESP32-C3 Schlafzimmer"
#define BLE_NAME "ESP32-C3 Kueche"

// ── Standort (wird auf OLED angezeigt) ───────────────────
// #define STANDORT "Balkon"
// #define STANDORT "Schlafzimmer"
#define STANDORT "Kueche"

// ── I2C Pins (nicht ändern) ──────────────────────────────
#define I2C_SDA 5
#define I2C_SCL 6

// ── Sensor I2C Adresse ───────────────────────────────────
#define SENSOR_ADDR 0x76

