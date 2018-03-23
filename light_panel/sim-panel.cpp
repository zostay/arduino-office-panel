#include <allegro5/allegro.h>
#include "sim-panel.h"

SimPanel::SimPanel() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
}

SimPanel::~SimPanel() { }

void SimPanel::set_brightness(int b) {
    // noop
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
            ALLEGRO_COLOR this_color = grid[cx][cy];
            al_draw_filled_circle(cx*160 + 80, cy*160.0 + 80, 40.0, this_color);
        }
    }

    al_flip_display();
}
