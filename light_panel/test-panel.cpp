#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "grid.h"
#include "sim-panel.h"
#include "piles.h"
#include "on_air.h"

ALLEGRO_COLOR grid[8][8];

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(0.06);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initailize keyboard!\n");
        return -1;
    }

    display = al_create_display(640, 640);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    SimPanel *panel = new SimPanel();
    Grid *current = new PileGrid(panel);

    while(1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            // tick
            current->start_loop();
            current->loop();
            current->finish_loop();
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_P:
                    delete current;
                    current = new PileGrid(panel);
                    break;
                case ALLEGRO_KEY_O:
                    delete current;
                    current = new OnAirGrid(panel, ON_AIR_PROGRAM);
                    break;
                case ALLEGRO_KEY_E:
                    delete current;
                    current = new OnAirGrid(panel, EMERGENCY_PROGRAM);
                    break;
            }
        }
    }

    delete current;
    delete panel;

    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}
