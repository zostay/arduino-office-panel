#ifndef SIM_PANEL_H
#define SIM_PANEL_H

#include "grid.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class AllegroColor : public PanelColor {
    private:
        ALLEGRO_COLOR color;

    public:
        AllegroColor(ALLEGRO_COLOR color) {
            this->color = color;
        }

        ALLEGRO_COLOR get_color() { return color; }
};

class SimPanel : public Panel {
    private:
        int brightness;
        ALLEGRO_COLOR grid[8][8];

    public:
        SimPanel();
        virtual ~SimPanel();

        virtual void set_brightness(int b);

        virtual PanelColor *mk_color_rgb(uint8_t r, uint8_t g, uint8_t b);
        virtual void set_color(int gx, int gy, PanelColor *color);

        virtual void start_loop();
        virtual void finish_loop();
};

#endif //SIM_PANEL_H
