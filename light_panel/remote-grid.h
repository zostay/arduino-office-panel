#ifndef REMOTE_GRID_H
#define REMOTE_GRID_H

#include "grid.h"

class RemoteGrid : public Grid {
    public:
        RemoteGrid(Panel *panel);

        virtual void loop(long tick);

        void remote_clear_panel(uint8_t r, uint8_t g, uint8_t b);
        void remote_set_color(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
        void remote_set_colors(const uint8_t *colors);
        void remote_set_brightness(uint8_t b);
};

#endif //REMOTE_GRID_H
