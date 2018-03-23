#ifndef GRID_H
#define GRID_H

#include <cstdint>

class PanelColor { };

class Panel {
    public:
        virtual void set_brightness(int b) = 0;

        virtual PanelColor *mk_color_rgb(uint8_t r, uint8_t g, uint8_t b) = 0;
        virtual void set_color(int gx, int gy, PanelColor *color) = 0;

        virtual void start_loop() = 0;
        virtual void finish_loop() = 0;
};

class Grid {
    private:
        Panel *panel;

    public:
        Grid(Panel *panel);
        virtual ~Grid();

        void set_brightness(int b) { panel->set_brightness(b); };

        PanelColor *mk_color_rgb(uint8_t r, uint8_t g, uint8_t b) { return panel->mk_color_rgb(r, g, b); }
        void set_color(int gx, int gy, PanelColor *color) { panel->set_color(gx, gy, color); }

        void start_loop() { panel->start_loop(); }
        void finish_loop() { panel->finish_loop(); }

        virtual void loop() = 0;
};

#endif //GRID_H
