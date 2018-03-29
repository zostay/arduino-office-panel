#include "remote-grid.h"

RemoteGrid::RemoteGrid(Panel *panel) : Grid(panel) { }

void RemoteGrid::loop(long tick) { }

void RemoteGrid::remote_clear_panel(uint8_t r, uint8_t g, uint8_t b) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            auto pix = mk_color_rgb(r, g, b);
            set_color(x, y, pix);
            delete pix;
        }
    }
}

void RemoteGrid::remote_set_color(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
    auto pix = mk_color_rgb(r, g, b);
    set_color(x, y, pix);
    delete pix;
}

void RemoteGrid::remote_set_colors(const uint8_t *colors) {
    int i;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            i = x * 3 + y * 24;
            uint8_t r = colors[i];
            uint8_t g = colors[i+1];
            uint8_t b = colors[i+2];

            auto pix = mk_color_rgb(r, g, b);
            set_color(x, y, pix);
            delete pix;
        }
    }
}

void RemoteGrid::remote_set_brightness(uint8_t brightness) {
    set_brightness(brightness);
}
