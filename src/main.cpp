#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// GPIO5 = SDA, GPIO6 = SCL
U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);

void setup() {
    Serial.begin(115200);
    while(!Serial) delay(10);
    delay(1000);
    
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Hello!");
    u8g2.drawStr(0, 25, "ESP32-C3");
    u8g2.drawStr(0, 40, "Mini");
    u8g2.sendBuffer();
    
    Serial.println("OLED gestartet!");
}

void loop() {
    delay(1000);
}