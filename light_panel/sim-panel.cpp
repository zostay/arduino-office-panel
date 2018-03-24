#include <allegro5/allegro.h>
#include <math.h>
#include "sim-panel.h"

SimPanel::SimPanel() {
    brightness = 128;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
}

SimPanel::~SimPanel() { }

void SimPanel::set_brightness(int b) {
    brightness = b;
}

PanelColor *SimPanel::mk_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return new AllegroColor(
       al_map_rgb(r, g, b)
    );
}

void SimPanel::set_color(int gx, int gy, PanelColor *pcolor) {
    AllegroColor *ncolor = static_cast<AllegroColor *>(pcolor);
    ALLEGRO_COLOR color = ncolor->get_color();
    grid[gx][gy] = color;
}

void SimPanel::start_loop() {
    al_clear_to_color(al_map_rgb(0,0,0));
}

void SimPanel::finish_loop() {
    for (int cy = 0; cy < 8; ++cy) {
        for (int cx = 0; cx < 8; ++cx) {
            unsigned char r, g, b;
            al_unmap_rgb(grid[cx][cy], &r, &g, &b);
            float log255 = log(255.0);
            r *= log(brightness+1) / log255;
            g *= log(brightness+1) / log255;
            b *= log(brightness+1) / log255;
            ALLEGRO_COLOR this_color = al_map_rgb(r, g, b);
            al_draw_filled_circle(cx*160 + 80, cy*160.0 + 80, 40.0, this_color);
        }
    }

    al_flip_display();
}
