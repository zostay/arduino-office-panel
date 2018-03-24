#ifndef ON_AIR_H
#define ON_AIR_H

#include "grid.h"

#define ON_AIR_PROGRAM    1
#define EMERGENCY_PROGRAM 2

class OnAirGrid : public Grid {
    private:
        PanelColor *color_red;
        PanelColor *color_blue;
        PanelColor *color_black;

        char prev_on_air_mode;
        char on_air_mode;
        int on_air_ptr;
        int urgency;

        unsigned char *program;

    public:
        OnAirGrid(Panel *panel, int program_selector);
        ~OnAirGrid();

        virtual void loop(long tick);

};

#endif //ON_AIR_H
