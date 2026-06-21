#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "config.h"

#ifdef SENSOR_BME280
    #include <Adafruit_BME280.h>
    Adafruit_BME280 bme;
#else
    #include <Adafruit_BMP280.h>
    Adafruit_BMP280 bme(&Wire);
#endif

// OLED
U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);

// BLE UUIDs
#define SERVICE_UUID  "12345678-1234-1234-1234-123456789abc"
#define TEMP_UUID     "12345678-1234-1234-1234-123456789ab1"
#define HUM_UUID      "12345678-1234-1234-1234-123456789ab2"
#define PRES_UUID     "12345678-1234-1234-1234-123456789ab3"

BLECharacteristic *tempChar;
BLECharacteristic *humChar;
BLECharacteristic *presChar;
bool deviceConnected = false;

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* server) {
        deviceConnected = true;
        Serial.println("BLE Client verbunden!");
    }
    void onDisconnect(BLEServer* server) {
        deviceConnected = false;
        Serial.println("BLE Client getrennt!");
        BLEDevice::startAdvertising();
    }
};

void setup() {
    Serial.begin(115200);
    while(!Serial) delay(10);
    delay(1000);

    // OLED
    u8g2.begin();
    Wire.begin(5, 6);
    delay(100);

    // Sensor initialisieren
    #ifdef SENSOR_BME280
        if (!bme.begin(SENSOR_ADDR, &Wire)) {
    #else
        if (!bme.begin(0x76, 0x58)) {
    #endif
            Serial.println("Sensor nicht gefunden!");
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_6x10_tf);
            u8g2.drawStr(0, 20, "Sensor");
            u8g2.drawStr(0, 32, "nicht OK!");
            u8g2.sendBuffer();
        } else {
            Serial.println("Sensor OK!");
        }

    // BLE initialisieren
    BLEDevice::init(BLE_NAME);
    BLEServer *server = BLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

    tempChar = service->createCharacteristic(
        TEMP_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    tempChar->addDescriptor(new BLE2902());

    humChar = service->createCharacteristic(
        HUM_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    humChar->addDescriptor(new BLE2902());

    presChar = service->createCharacteristic(
        PRES_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    presChar->addDescriptor(new BLE2902());

    service->start();
    BLEDevice::startAdvertising();

    Serial.printf("BLE Advertising: %s\n", BLE_NAME);

    // Startanzeige OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, BLE_NAME);
    u8g2.drawStr(0, 22, STANDORT);
    u8g2.drawStr(0, 34, "BLE OK");
    u8g2.sendBuffer();
    delay(2000);
}

void loop() {
    float temp = bme.readTemperature();
    float pres = bme.readPressure() / 100.0F;

#ifdef SENSOR_BME280
    float hum = bme.readHumidity();
#else
    float hum = 0.0;
#endif

    char buf[10];
    sprintf(buf, "%.1f", temp);
    tempChar->setValue(buf);
    tempChar->notify();

    sprintf(buf, "%.1f", hum);
    humChar->setValue(buf);
    humChar->notify();

    sprintf(buf, "%.1f", pres);
    presChar->setValue(buf);
    presChar->notify();

    Serial.printf("T: %.1f C  H: %.1f%%  P: %.1f hPa  [%s]\n", temp, hum, pres, STANDORT);

    // OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, STANDORT);

    char line[20];
    sprintf(line, "T: %.1f C", temp);
    u8g2.drawStr(0, 22, line);

#ifdef SENSOR_BME280
    sprintf(line, "H: %.1f %%", hum);
    u8g2.drawStr(0, 32, line);
    sprintf(line, "P:%.0f hPa", pres);
    u8g2.drawStr(0, 42, line);
#else
    sprintf(line, "P:%.0f hPa", pres);
    u8g2.drawStr(0, 32, line);
    u8g2.drawStr(0, 42, "kein Hygro");
#endif

    u8g2.sendBuffer();
    delay(2000);
}