#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_COLOR grid[8][8];

void set_color(int gx, int gy, ALLEGRO_COLOR color) {
    grid[gx][gy] = color;
}

void set_color_rgb(int gx, int gy, int r, int g, int b) {
    grid[gx][gy] = al_map_rgb(r, g, b);
}

void set_color_hsv(int gx, int gy, float h, float s, float v) {
    while (h < 0.0) h += 360.0;

    float hp = h / 60.0;
    float c = v * s;
    float x = c * (1.0 - fabs(fmod(hp, 2.0) - 1.0));
    float m = v - c;

    float r1, g1, b1;
    if (hp >= 0.0 && hp < 1.0) {
        r1 = c; g1 = x; b1 = 0.0;
    }
    else if (hp >= 1.0 && hp < 2.0) {
        r1 = x; g1 = c; b1 = 0.0;
    }
    else if (hp >= 2.0 && hp < 3.0) {
        r1 = 0.0; g1 = c; b1 = x;
    }
    else if (hp >= 3.0 && hp < 4.0) {
        r1 = 0.0; g1 = x; b1 = c;
    }
    else if (hp >= 4.0 && hp < 5.0) {
        r1 = x; g1 = 0.0; b1 = c;
    }
    else {
        r1 = c; g1 = 0.0; b1 = x;
    }

    unsigned char r = (r1 + m) * 255;
    unsigned char g = (g1 + m) * 255;
    unsigned char b = (b1 + m) * 255;

    set_color_rgb(gx, gy, r, g, b);
}

typedef struct {
    float h, s, v;
} hsv_color;

void set_color_hsv_struct(int gx, int gy, hsv_color *c) {
    set_color_hsv(gx, gy, c->h, c->s, c->v);
}

float frand(float a) {
    return ((float)rand()/(float)(RAND_MAX)) * a;
}

int tent_x, tent_y;
hsv_color tent_color;
hsv_color hsv_grid[8][8];
hsv_color tent_inc = { .h=1.0, .s=0.01, .v=0.01 };
void randomize_color_tent() {
    tent_color.h = frand(360.0);
    tent_color.s = frand(1.0);
    tent_color.v = frand(1.0);
}

void setup_color_tent() {
    randomize_color_tent();

    tent_x = rand() % 8;
    tent_y = rand() % 8;

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            hsv_grid[gx][gy].h = 0.0;
            hsv_grid[gx][gy].s = 0.0;
            hsv_grid[gx][gy].v = 0.0;
        }
    }
}

ALLEGRO_COLOR pile_colors[4];
int piles_grid[8][8];
void setup_piles() {
    pile_colors[0] = al_map_rgb(0, 0, 0);
    pile_colors[1] = al_map_rgb(0, 0, 255);
    pile_colors[2] = al_map_rgb(255, 255, 0);
    pile_colors[3] = al_map_rgb(255, 0, 0);

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            piles_grid[gx][gy] = 0;
        }
    }
}

ALLEGRO_COLOR color_red;
void setup_on_air() {
    color_red = al_map_rgb(255, 0, 0);
}

void setup() {
    setup_color_tent();
    setup_piles();
    setup_on_air();
}

float counter = 0.0;
void flat_smooth() {
    counter = fmod(counter + 1, 360.0);
    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            set_color_hsv(gx, gy, counter, 1.0, 1.0);
        }
    }
}

#define TOROID(c) c < 0 ? c+8 : c > 7 ? c-8 : c

int color_tent_counter = 0;
void color_tent() {
    color_tent_counter++;

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            if (gx == tent_x && gy == tent_y) {
                bool lock = true;

                if (hsv_grid[gx][gy].h + tent_inc.h < tent_color.h) {
                    hsv_grid[gx][gy].h += tent_inc.h;
                    lock = false;
                }
                else if (hsv_grid[gx][gy].h - tent_inc.h > tent_color.h) {
                    hsv_grid[gx][gy].h -= tent_inc.h;
                    lock = false;
                }

                if (hsv_grid[gx][gy].s + tent_inc.s < tent_color.s) {
                    hsv_grid[gx][gy].s += tent_inc.s;
                    lock = false;
                }
                else if (hsv_grid[gx][gy].s - tent_inc.s > tent_color.s) {
                    hsv_grid[gx][gy].s -= tent_inc.s;
                    lock = false;
                }

                if (hsv_grid[gx][gy].v + tent_inc.v < tent_color.v) {
                    hsv_grid[gx][gy].v += tent_inc.v;
                    lock = false;
                }
                else if (hsv_grid[gx][gy].v - tent_inc.v > tent_color.v) {
                    hsv_grid[gx][gy].v -= tent_inc.v;
                    lock = false;
                }

                if (lock)
                    randomize_color_tent();

                printf("(%d, %d): %5.1f, %4.2f, %4.2f -> %5.1f, %4.2f, %4.2f\n",
                    gx, gy, 
                    hsv_grid[gx][gy].h,
                    hsv_grid[gx][gy].s,
                    hsv_grid[gx][gy].v,
                    tent_color.h,
                    tent_color.s,
                    tent_color.v
                );
            }
            else if (color_tent_counter % 2 == 0) {
                hsv_color *n, *s, *e, *w;
                n = &hsv_grid[gx][TOROID(gy-1)];
                s = &hsv_grid[gx][TOROID(gy+1)];
                e = &hsv_grid[TOROID(gx+1)][gy];
                w = &hsv_grid[TOROID(gx-1)][gy];

                hsv_grid[gx][gy].h = (n->h + s->h + e->h + w->h)/4.0;
                hsv_grid[gx][gy].s = (n->s + s->s + e->s + w->s)/4.0;
                hsv_grid[gx][gy].v = (n->v + s->v + e->v + w->v)/4.0;
            }

            set_color_hsv_struct(gx, gy, &hsv_grid[gx][gy]);
        }
    }

    if (color_tent_counter > 100) {
        tent_x = TOROID(tent_x + rand() % 3 - 2);
        tent_y = TOROID(tent_y + rand() % 3 - 2);
        color_tent_counter = 0;
    }
}

int piles_counter = 0;
void piles() {
    piles_grid[rand()%8][rand()%8]++;

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            int *here = &piles_grid[gx][gy];

            printf("%d", *here);

            if (*here > 3) {
                printf("- ");
                
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
                printf("  ");
            }
        }
        printf("\n");
    }

    printf("\n");

    for (int gy = 0; gy < 8; ++gy) {
        for (int gx = 0; gx < 8; ++gx) {
            int *here = &piles_grid[gx][gy];
            set_color(gx, gy, pile_colors[*here % 4]);
        }
    }
}

unsigned char O[] = {
    0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7e,
};

unsigned char on_air_program[] = {
    0x00, 0x00, 0x01,
    0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    0x00, 0x00, 0x02,
    0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    0x00, 0x00, 0x01,
    0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    0x00, 0x00, 0x02,
    0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    0x00, 0x00, 0x03,
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, // O
    0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3, // N
    0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, // A
    0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, // I
    0xfc, 0x63, 0x63, 0x7e, 0x7e, 0x63, 0x63, 0xe3, // R
    0x00, 0x00, 0x81,
    0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 
    0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 
    0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 
    0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0x00, 0x82,
    0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 
    0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 
    0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 
    0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f,
    0x00, 0x00, 0x03,
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, // O
    0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3, // N
    0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, // A
    0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, // I
    0xfc, 0x63, 0x63, 0x7e, 0x7e, 0x63, 0x63, 0xe3, // R
    0x00, 0x00, 0x00,
};
char on_air_mode = 0;
int on_air_ptr = 0;
int underclock = 0;
int urgency = 10;
void on_air() {
    if (underclock > 0) {
        underclock--;
        return;
    }

    if (on_air_mode & 0x80)
        urgency = 2;
    else
        urgency = 10;
    
    underclock = urgency;


    if (on_air_program[on_air_ptr] == 0
            && on_air_program[on_air_ptr+1] == 0
            && on_air_program[on_air_ptr+2] == 0) {
        on_air_ptr = 0;
    }

    if (on_air_program[on_air_ptr] == 0
            && on_air_program[on_air_ptr+1] == 0) {
        on_air_mode = on_air_program[on_air_ptr+2];
        on_air_ptr += 3;
    }

    unsigned char op = on_air_program[on_air_ptr++];
    unsigned char mode_type = on_air_mode & 0x0F;
    if (mode_type == 0x01) { // vert red/blue
        for (int gy = 0; gy < 8; ++gy) {
            for (unsigned int gx = 0, mask = 0x80; gx < 8; ++gx, mask >>= 1) {
                printf("%02x & %02x\n", op, mask);
                if (op & mask && gx < 4)
                    set_color_rgb(gx, gy, 255, 0, 0);
                else if (op & mask)
                    set_color_rgb(gx, gy, 0, 0, 255);
                else
                    set_color_rgb(gx, gy, 0, 0, 0);
            }
        }
    }

    else if (mode_type == 0x02) { // horz red/blue
        for (int gx = 0; gx < 8; ++gx) {
            for (unsigned int gy = 0, mask = 0x80; gy < 8; ++gy, mask >>= 1) {
                if (op & mask && gy < 4)
                    set_color_rgb(gx, gy, 255, 0, 0);
                else if (op & mask)
                    set_color_rgb(gx, gy, 0, 0, 255);
                else
                    set_color_rgb(gx, gy, 0, 0, 0);
            }
        }
    }
    
    else if (mode_type == 0x03) { // red bitmap
        for (int gy = 0; gy < 8; ++gy) {
            for (unsigned int gx = 0, mask = 0x80; gx < 8; ++gx, mask >>= 1) {
                if (op & mask) {
                    set_color_rgb(gx, gy, 255, 0, 0);
                    printf("#");
                }
                else {
                    set_color_rgb(gx, gy, 0, 0, 0);
                    printf(" ");
                }
            }
            printf("\n");

            if (gy < 7)
                op = on_air_program[on_air_ptr++];
        }
        printf("\n");
    }

    else {
        for (int gy = 0; gy < 8; ++gy) {
            for (int gx = 0; gx < 8; ++gx) {
                set_color_rgb(gx, gy, 255, 0, 0);
            }
        }
    }
}

void loop() {
    //flat_smooth();
    //color_tent();
    piles();
    //on_air();
}

int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(640, 640);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    setup();

    while(1) {
        ALLEGRO_EVENT ev;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.06);

        bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);

        if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        al_clear_to_color(al_map_rgb(0,0,0));

        loop();

        for (int cy = 0; cy < 8; ++cy) {
            for (int cx = 0; cx < 8; ++cx) {
                ALLEGRO_COLOR this_color = grid[cx][cy];
                al_draw_filled_circle(cx*160 + 80, cy*160.0 + 80, 40.0, this_color);
            }
        }

        al_flip_display();
    }

    al_destroy_display(display);

    return 0;
}
