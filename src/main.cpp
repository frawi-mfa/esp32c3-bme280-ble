#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// OLED
U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);

// BME280
Adafruit_BME280 bme;

// BLE UUIDs
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define TEMP_UUID           "12345678-1234-1234-1234-123456789ab1"
#define HUM_UUID            "12345678-1234-1234-1234-123456789ab2"
#define PRES_UUID           "12345678-1234-1234-1234-123456789ab3"

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

    // BME280
    if (!bme.begin(0x76, &Wire)) {
        Serial.println("BME280 nicht gefunden!");
    }

    // BLE initialisieren
    BLEDevice::init("ESP32-C3 Wetterstation");
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

    Serial.println("BLE Advertising gestartet!");
}

void loop() {
    float temp = bme.readTemperature();
    float hum  = bme.readHumidity();
    float pres = bme.readPressure() / 100.0F;

    // BLE Werte aktualisieren
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

    Serial.printf("T: %.1f C  H: %.1f%%  P: %.1f hPa\n", temp, hum, pres);

    // OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);

    char line[20];
    sprintf(line, "T: %.1f C", temp);
    u8g2.drawStr(0, 12, line);

    sprintf(line, "H: %.1f %%", hum);
    u8g2.drawStr(0, 25, line);

    sprintf(line, "P:%.0f hPa", pres);
    u8g2.drawStr(0, 38, line);

    u8g2.sendBuffer();
    delay(2000);
}