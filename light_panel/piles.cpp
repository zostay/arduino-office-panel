#include <cstdlib>
#include <unistd.h>
#include "piles.h"

#define BRIGHTNESS 20

#define delay(ms) sleep(ms)

PileGrid::PileGrid(Panel *panel) : Grid(panel) {
    piles_counter = 0;

    pile_colors[0] = mk_color_rgb(0, 0, 0);
    pile_colors[1] = mk_color_rgb(0, 0, 255);
    pile_colors[2] = mk_color_rgb(255, 255, 0);
    pile_colors[3] = mk_color_rgb(255, 0, 0);

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            piles_grid[gx][gy] = 0;
        }
    }

    set_brightness(BRIGHTNESS);
}

PileGrid::~PileGrid() {
    for (int i = 0; i < 4; ++i) {
        delete pile_colors[i];
    }
}

void PileGrid::loop() {
    piles_grid[rand()%8][rand()%8]++;
    //piles_grid[0][0]++; //boring
    //if (++piles_counter % 4) {
    //    piles_grid[3][3]++;
    //}
    //else if (piles_counter % 4 == 1) {
    //    piles_grid[3][4]++;
    //}
    //else if (piles_counter % 4 == 2) {
    //    piles_grid[4][3]++;
    //}
    //else {
    //    piles_grid[4][4]++;
    //}
    // printf("piles_grid[0][0] = %d\n", piles_grid[0][0]);

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            int *here = &piles_grid[gx][gy];

            //printf("%d", *here);

            if (*here > 3) {
                //printf("- ");

                (*here) -= 4;
                if (gy > 0)
                    piles_grid[gx][gy-1]++;
                if (gy < 7)
                    piles_grid[gx][gy+1]++;
                if (gx > 0)
                    piles_grid[gx-1][gy]++;
                if (gx < 7)
                    piles_grid[gx+1][gy]++;
            }
            else {
                //printf("  ");
            }

            // unsigned char r, g, b;
            // al_unmap_rgb(pile_colors[here % 4], &r, &g, &b);
            // printf("(%d, %d): [%d] %3d, %3d, %3d\n", gx, gy, here % 4, r, g, b);
        }
        //printf("\n");
    }

    //printf("\n");

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            int *here = &piles_grid[gx][gy];
            set_color(gx, gy, pile_colors[*here % 4]);
        }
    }

    delay(100);
}
