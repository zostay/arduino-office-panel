#ifndef GRID_H
#define GRID_H

#include <Adafruit_NeoMatrix.h>

class Grid {
    private:
        Adafruit_NeoMatrix *panel;

    public:
        Grid();
        ~Grid();

        void set_brightness(int b);

        uint16_t mk_color(uint8_t r, uint8_t g, uint8_t b);
        void set_color(int gx, int gy, uint16_t color);

        void finish_loop();

        virtual void loop() = 0;
};

#endif //GRID_H
