#ifndef PILES_H
#define PILES_H

#include "grid.h"

class PileGrid : public Grid {
    private:
        uint16_t pile_colors[4];
        int piles_grid[8][8];

        int piles_counter = 0;

    public:
        PileGrid();

        virtual void loop();
};

#endif //PILES_H
