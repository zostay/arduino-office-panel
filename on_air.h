#ifndef ON_AIR_H
#define ON_AIR_H

#include "grid.h"

#define ON_AIR_PROGRAM    1
#define EMERGENCY_PROGRAM 2

class OnAirGrid : public Grid {
    private:
        uint16_t color_red;
        uint16_t color_blue;
        uint16_t color_black;

        char prev_on_air_mode = 0;
        char on_air_mode = 0;
        int on_air_ptr = 0;
        int underclock = 0;
        int urgency = 50;

        char *program;

    public:
        OnAirGrid(int program_selector);

        virtual void loop();

};

#endif //ON_AIR_H
