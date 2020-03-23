/**
 * Examples from
 * - https://breakrow.com/miliohm/non-contact-temperature-sensor-mlx90614-with-arduino-tutorial/
 * - https://github.com/adafruit/Adafruit_LED_Backpack
 * - http://quantum.engr.tu.ac.th/smf/index.php?topic=94.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_MLX90614.h>

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
Adafruit_MLX90614       mlx    = Adafruit_MLX90614();

#define LM73_ADDR 0x4D

void setup() {
    while (!Serial)
        ;
    Serial.begin(115200);
    Serial.println("16x8 LED Mini Matrix Test");

    matrix.begin(0x70);  // pass in the address
    mlx.begin();
    Wire1.begin(4, 5);  // for LM73. SDA1=IO4 SCL1=IO5
}

void loop() {
    double        temp_room = mlx.readAmbientTempC();
    double        temp_obj  = mlx.readObjectTempC();
    double        temp_lm   = readTemperature();
    double        temp_adj  = temp_obj + (temp_lm - temp_room);
    static double temp_last = 0.;
    // Serial.print(mlx.readAmbientTempF());
    // Serial.print(mlx.readObjectTempF());

    Serial.print("Ambient:");
    Serial.print(temp_room);
    Serial.print("°C, Obj:");
    Serial.print(temp_obj);
    Serial.print("°C, LM73:");
    Serial.print(temp_lm);
    Serial.print("°C, Adj:");
    Serial.print(temp_adj);
    Serial.print("°C");
    Serial.println();
    delay(100);

    if (abs(temp_adj - temp_last) < 0.9) {
        matrix.setTextSize(1);
        matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
        matrix.setTextColor(LED_ON);
        matrix.setRotation(1);

        for (int8_t x = 0; x >= -16; x--) {
            matrix.clear();
            matrix.setCursor(x, 0);
            matrix.print(temp_adj);
            matrix.writeDisplay();
            delay((x == 0) ? 500 : 100);
        }
    }

    temp_last = temp_adj;
}

float readTemperature() {
    // Example: http://quantum.engr.tu.ac.th/smf/index.php?topic=94.0
    Wire1.beginTransmission(LM73_ADDR);
    Wire1.write(0x00);  // Temperature Data Register
    Wire1.endTransmission();

    uint8_t count = Wire1.requestFrom(LM73_ADDR, 2);
    float   temp  = 0.0;
    if (count == 2) {
        byte buff[2];
        buff[0] = Wire1.read();
        buff[1] = Wire1.read();
        temp += (int)(buff[0] << 1);
        if (buff[1] & 0b10000000) temp += 1.0;
        if (buff[1] & 0b01000000) temp += 0.5;
        if (buff[1] & 0b00100000) temp += 0.25;
        if (buff[0] & 0b10000000) temp *= -1.0;
    }

    return temp;
}
