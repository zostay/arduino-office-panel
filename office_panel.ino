#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoMatrix panel = Adafruit_NeoMatrix(8, 8, 2,
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
    NEO_GRBW + NEO_KHZ800);

uint16_t grid[8][8];

void set_color(int gx, int gy, uint16_t color) {
    grid[gx][gy] = color;
}

uint16_t pile_colors[4];
int piles_grid[8][8];
void setup_piles() {
    pile_colors[0] = panel.Color(0, 0, 0);
    pile_colors[1] = panel.Color(0, 0, 255);
    pile_colors[2] = panel.Color(255, 255, 0);
    pile_colors[3] = panel.Color(255, 0, 0);

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            piles_grid[gx][gy] = 0;
        }
    }
}

void setup() {
    panel.begin();
    panel.setBrightness(40);
    setup_piles();
}

int piles_counter = 0;
void piles() {
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
}

void loop() {
  panel.fillScreen(0);
  piles();

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      panel.drawPixel(x, y, grid[x][y]);
    }
  }
  panel.show();
  delay(100);
}
