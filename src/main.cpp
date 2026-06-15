#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// OLED und BME280 teilen GPIO5/6
U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);
Adafruit_BME280 bme;

void setup() {
    Serial.begin(115200);
    while(!Serial) delay(10);
    delay(1000);

    // OLED zuerst
    u8g2.begin();
    
    // Wire NACH u8g2.begin() neu setzen!
    Wire.begin(5, 6);

    // BME280 starten
    if (!bme.begin(0x76, &Wire)) {
        Serial.println("BME280 nicht gefunden!");
    } else {
        Serial.println("BME280 OK!");
    }
}
void loop() {
    float temp = bme.readTemperature();
    float hum  = bme.readHumidity();
    float pres = bme.readPressure() / 100.0F;

    Serial.printf("T: %.1f C  H: %.1f%%  P: %.1f hPa\n", temp, hum, pres);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);

    char buf[20];
    sprintf(buf, "T: %.1f C", temp);
    u8g2.drawStr(0, 12, buf);

    sprintf(buf, "H: %.1f %%", hum);
    u8g2.drawStr(0, 25, buf);

    sprintf(buf, "P:%.0f hPa", pres);
    u8g2.drawStr(0, 38, buf);

    u8g2.sendBuffer();
    delay(2000);
}