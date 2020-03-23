/**
 * Examples from
 * - https://breakrow.com/miliohm/non-contact-temperature-sensor-mlx90614-with-arduino-tutorial/
 * - https://github.com/adafruit/Adafruit_LED_Backpack
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
// #include <Adafruit_MLX90614.h>
#include <SparkFunMLX90614.h>

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
// Adafruit_MLX90614       mlx    = Adafruit_MLX90614();
IRTherm therm;

void setup() {
    while (!Serial)
        ;
    Serial.begin(115200);
    Serial.println("16x8 LED Mini Matrix Test");

    matrix.begin(0x70);  // pass in the address

    // mlx.begin();
    therm.begin();
    therm.setUnit(TEMP_C);
}

void loop() {
    delay(100);

    double temp_room;
    double temp_obj;

    if (therm.read())  // On success, read() will return 1, on fail 0.
    {
        temp_room = therm.ambient();  // Get updated ambient temperature
        temp_obj  = therm.object();   // Get updated object temperature

        Serial.print("Ambient = ");
        Serial.print(temp_room);
        Serial.print("*C\tObject = ");
        Serial.print(temp_obj);
        Serial.println("*C");

        matrix.setTextSize(1);
        matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
        matrix.setTextColor(LED_ON);
        matrix.setRotation(1);

        for (int8_t x = 0; x >= -16; x--) {
            matrix.clear();
            matrix.setCursor(x, 0);
            matrix.print(temp_obj);
            matrix.writeDisplay();
            delay((x == 0) ? 500 : 100);
        }
    }
}
