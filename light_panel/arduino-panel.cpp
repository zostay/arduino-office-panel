#include "arduino-panel.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

ArduinoPanel::ArduinoPanel() {
    panel = new Adafruit_NeoMatrix(8, 8, 2,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
        NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRBW + NEO_KHZ800
    );

    panel->begin();
}

ArduinoPanel::~ArduinoPanel() {
    delete panel;
}

PanelColor *ArduinoPanel::mk_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return new NeopixelColor(
        panel->Color(r, g, b)
    );
}

void ArduinoPanel::set_color(int gx, int gy, PanelColor *pcolor) {
    uint16_t color = static_cast<NeopixelColor *>(pcolor)->get_color();
    panel->drawPixel(gx, gy, color);
}

void ArduinoPanel::finish_loop() {
    panel->show();
}

void ArduinoPanel::set_brightness(int b) {
    panel->setBrightness(b);
}
