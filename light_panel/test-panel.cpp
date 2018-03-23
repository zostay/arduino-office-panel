#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

#include "grid.h"
#include "sim-panel.h"
#include "piles.h"
#include "on_air.h"

ALLEGRO_COLOR grid[8][8];

int main(int argc, char **argv) {
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if(!al_init()) {
        std::cerr << "failed to initialize allegro!" << std::endl;
        return -1;
    }

    std::unique_ptr<ALLEGRO_TIMER, void(*)(ALLEGRO_TIMER*)> timer(
        al_create_timer(0.06),
        al_destroy_timer
    );
    if (!timer) {
        std::cerr << "failed to create timer!" << std::endl;
        return -1;
    }

    if (!al_install_keyboard()) {
        std::cerr << "failed to initailize keyboard!" << std::endl;
        return -1;
    }

    std::unique_ptr<ALLEGRO_DISPLAY, void(*)(ALLEGRO_DISPLAY*)> display(
        al_create_display(640, 640),
        al_destroy_display
    );
    if(!display) {
        std::cerr << "failed to create display!" << std::endl;
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        std::cerr << "failed to create event_queue!" << std::endl;
        return -1;
    }

    if (!al_init_primitives_addon()) {
        std::cerr << "failed to initialize primitives!" << std::endl;
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display.get()));
    al_register_event_source(event_queue, al_get_timer_event_source(timer.get()));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    SimPanel *panel = new SimPanel();
    Grid *current = new PileGrid(panel);

    al_start_timer(timer.get());

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

    return 0;
}
