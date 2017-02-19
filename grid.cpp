#include "grid.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

Grid::Grid() {
    panel = new Adafruit_NeoMatrix(8, 8, 2,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
        NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRBW + NEO_KHZ800
    );

    panel->begin();
}

Grid::~Grid() {
    delete panel;
}

uint16_t Grid::mk_color(uint8_t r, uint8_t g, uint8_t b) {
    return panel->Color(r, g, b);
}

void Grid::set_color(int gx, int gy, uint16_t color) {
    panel->drawPixel(gx, gy, color);
}

void Grid::finish_loop() {
    panel->show();
}

void Grid::set_brightness(int b) {
    panel->setBrightness(b);
}
