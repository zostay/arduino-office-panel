#ifndef PILES_H
#define PILES_H

#include "grid.h"

class PileGrid : public Grid {
    private:
        PanelColor *pile_colors[4];
        int piles_grid[8][8];

        int piles_counter;

    public:
        PileGrid(Panel *panel);
        ~PileGrid();

        virtual void loop();
};

#endif //PILES_H
