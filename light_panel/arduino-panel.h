#ifndef ARDUINO_PANEL_H
#define ARDUINO_PANEL_H

#include "grid.h"
#include <Adafruit_NeoMatrix.h>

class NeopixelColor : public PanelColor {
    private:
        uint16_t color;

    public:
        NeopixelColor(uint16_t color) {
            this.color = color;
        }

        uint16_t get_color() { return color; }
}

class ArduinoPanel : public Panel {
    private:
        Adafruit_NeoMatrix *panel;

    public:
        ArduinoPanel();
        virtual ~ArduinoPanel();

        virtual void set_brightness(int b);

        virtual PanelColor *mk_color_rgb(uint8_t r, uint8_t g, uint8_t b);
        virtual void set_color(int gx, int gy, PanelColor *color);

        virtual void start_loo() {}
        virtual void finish_loop();
}

#endif //ARDUINO_PANEL_H
