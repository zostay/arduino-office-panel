#include <math.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

#include "udp-listener.h"
#include "grid.h"
#include "sim-panel.h"
#include "piles.h"
#include "on_air.h"
#include "remote-grid.h"

// C++ sucks butt on formatting... there's a better way to do this using a tiny
// little class and custom operator... BUT OMGOSH ALREADY
#define hex(c) std::setw(2) << std::setfill('0') << std::hex << (int)(char)c
#define dec(c) std::dec << (int)(char)c

ALLEGRO_COLOR grid[8][8];

const int TICK_HZ  = 20;
const int UDP_PORT = 10101;

int main(int argc, char **argv) {
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if(!al_init()) {
        std::cerr << "failed to initialize allegro!" << std::endl;
        return -1;
    }

    std::unique_ptr<ALLEGRO_TIMER, void(*)(ALLEGRO_TIMER*)> timer(
        al_create_timer(1.0 / TICK_HZ),
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

    SimPanel panel;
    std::unique_ptr<Grid> current = std::make_unique<PileGrid>(&panel);
    UdpListener udp(UDP_PORT);

    try {
        udp.listen();
        std::cerr << "Listening for UDP packets on port " << UDP_PORT << " ... " << std::endl;
    }
    catch (UdpListenerException e) {
        std::cerr << "Failed to listen on port " << UDP_PORT << ": " << e << std::endl;
        return 1;
    }

    al_start_timer(timer.get());

    bool done = false;
    while(!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            try {
                if (udp.message_ready()) {
                    std::string message(udp.receive_message());
                    if (message == std::string("\x00", 1)) {
                        std::cerr << "PileGrid" << std::endl;
                        current = std::make_unique<PileGrid>(&panel);
                    }
                    else if (message == "\x01") {
                        std::cerr << "OnAirGrid(ON_AIR_PROGRAM)" << std::endl;
                        current = std::make_unique<OnAirGrid>(&panel, ON_AIR_PROGRAM);
                    }
                    else if (message == "\x02") {
                        std::cerr << "OnAirGrid(EMERGENCY_PROGRAM)" << std::endl;
                        current = std::make_unique<OnAirGrid>(&panel, EMERGENCY_PROGRAM);
                    }
                    else if (message.find("\x03") == 0) {
                        std::cerr << "OnAirGrid(CUSTOM)" << std::endl;
                        current = std::make_unique<OnAirGrid>(&panel, reinterpret_cast<const unsigned char*>(message.data() + 1), message.length() - 1);
                    }
                    else if (message.find("\x11") == 0 && message.length() == 4) {
                        char r = message.at(1);
                        char g = message.at(2);
                        char b = message.at(3);

                        std::cerr << "RemoteGrid.clear(" 
                                          << hex(r)
                                  << ", " << hex(g)
                                  << ", " << hex(b)
                                  << ")"  << std::endl;
                        if (typeid(current) != typeid(RemoteGrid)) {
                            current = std::make_unique<RemoteGrid>(&panel);
                        }

                        dynamic_cast<RemoteGrid*>(current.get())->remote_clear_panel(r, g, b);
                    }
                    else if (message.find("\x12") == 0 && message.length() == 6) {
                        char x = message.at(1);
                        char y = message.at(2);
                        char r = message.at(3);
                        char g = message.at(4);
                        char b = message.at(5);

                        std::cerr << "RemoteGrid.set(" 
                                          << dec(x)
                                  << ", " << dec(y)
                                  << ", " << hex(r)
                                  << ", " << hex(g)
                                  << ", " << hex(b)
                                  << ")"  << std::endl;

                        if (typeid(current) != typeid(RemoteGrid)) {
                            current = std::make_unique<RemoteGrid>(&panel);
                        }

                        dynamic_cast<RemoteGrid*>(current.get())->remote_set_color(x, y, r, g, b);
                    }
                    else if (message.find("\x13") == 0 && message.length() == 193) {
                        std::cerr << "RemoteGrid.bitmap(...)" << std::endl;

                        if (typeid(current) != typeid(RemoteGrid)) {
                            current = std::make_unique<RemoteGrid>(&panel);
                        }

                        dynamic_cast<RemoteGrid*>(current.get())->remote_set_colors(reinterpret_cast<const uint8_t*>(message.data()+1));
                    }
                    else if (message.find("\x14") == 0 && message.length() == 2) {
                        char b = message.at(1);

                        std::cerr << "RemoteGrid.brightness(" << hex(b) << ")" << std::endl;

                        if (typeid(current) != typeid(RemoteGrid)) {
                            current = std::make_unique<RemoteGrid>(&panel);
                        }

                        dynamic_cast<RemoteGrid*>(current.get())->remote_set_brightness(b);
                    }
                }
            }

            catch  (UdpListenerException e) {
                std::cerr << "Unable to receive UDP message: " << e << std::endl;
                return 1;
            }

            // tick
            current->start_loop();
            current->loop(ev.timer.count);
            current->finish_loop();
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {

            case ALLEGRO_KEY_P:
                current = std::make_unique<PileGrid>(&panel);
                break;
                
            case ALLEGRO_KEY_O:
                current = std::make_unique<OnAirGrid>(&panel, ON_AIR_PROGRAM);
                break;

            case ALLEGRO_KEY_E:
                current = std::make_unique<OnAirGrid>(&panel, EMERGENCY_PROGRAM);
                break;

            case ALLEGRO_KEY_Q:
                done = true;
                break;

            }
        }
    }

    return 0;
}
