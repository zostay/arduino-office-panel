#include "on_air.h"

#define BRIGHTNESS 255

#define delay(ms)

unsigned char on_air_program[] = {
    0x00, 0x00, 0x84,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
    0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 
    0x00, 0x00, 0x03,
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, // O
    0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3, // N
    0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, // A
    0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, // I
    0xfc, 0x63, 0x63, 0x7e, 0x7e, 0x63, 0x63, 0xe3, // R
    0x00, 0x00, 0x84,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
    0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
    0x00, 0x00, 0x00,
};

unsigned char emergency_program[] = {
    // 0x00, 0x00, 0x81,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x00, 0x82,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x81, 0xc3, 0xe7, 0xff, 0x00, 0xff, 0x00, 0xff,
    // 0x00, 0x00, 0x81,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0x00, 0x82,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f,
    // 0x00, 0x00, 0x03,
    // 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, // O
    // 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3, // N
    // 0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, // A
    // 0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, // I
    // 0xfc, 0x63, 0x63, 0x7e, 0x7e, 0x63, 0x63, 0xe3, // R
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
    0x00, 0x00, 0x81,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0x33, 0x00, 0x33, 0x00, 0x33,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0x33, 0x00, 0x33, 0x00, 0x33,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0x33, 0x00, 0x33, 0x00, 0x33,
    0x00, 0x00, 0x82,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0x33, 0x00, 0x33, 0x00, 0x33,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0x33, 0x00, 0x33, 0x00, 0x33,
    0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc,
    0x00, 0x33, 0x00, 0x33, 0x00, 0x33,
    0x00, 0x00, 0x03,
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, // O
    0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3, // N
    0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, // A
    0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, // I
    0xfc, 0x63, 0x63, 0x7e, 0x7e, 0x63, 0x63, 0xe3, // R
    0x00, 0x00, 0x81,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x00, 0x82,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x80, 0x02, 0x20, 0x04, 0x40, 0x01, 0x10, 0x08,
    0x00, 0x00, 0x03,
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, // O
    0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0xc3, // N
    0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, // A
    0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, // I
    0xfc, 0x63, 0x63, 0x7e, 0x7e, 0x63, 0x63, 0xe3, // R
    0x00, 0x00, 0x00,
};

OnAirGrid::OnAirGrid(Panel *panel, int program_selector) : Grid(panel) {
    prev_on_air_mode = 0;
    on_air_mode = 0;
    on_air_ptr = 0;
    underclock = 0;
    urgency = 50;

    color_red   = mk_color_rgb(255, 0, 0);
    color_blue  = mk_color_rgb(0, 0, 255);
    color_black = mk_color_rgb(0, 0, 0);

    switch (program_selector) {

    case ON_AIR_PROGRAM:
        program = on_air_program;
        break;

    case EMERGENCY_PROGRAM:
        program = emergency_program;
        break;

    default:
        program = on_air_program;
        break;

    }

    set_brightness(BRIGHTNESS);
}

OnAirGrid::~OnAirGrid() {
    delete color_red;
    delete color_blue;
    delete color_black;
}

void OnAirGrid::loop() {
    if (on_air_mode & 0x80)
        urgency = 50;
    else
        urgency = 500;
    
    underclock = urgency;

    if (program[on_air_ptr] == 0
            && program[on_air_ptr+1] == 0
            && program[on_air_ptr+2] == 0) {
        on_air_ptr = 0;
    }

    if (program[on_air_ptr] == 0
            && program[on_air_ptr+1] == 0) {
        prev_on_air_mode = on_air_mode;
        on_air_mode = program[on_air_ptr+2];
        on_air_ptr += 3;

        set_brightness(BRIGHTNESS);
    }

    unsigned char op = program[on_air_ptr++];
    unsigned char mode_type = on_air_mode & 0x0F;
    if (mode_type == 0x01) { // vert red/blue
        for (int gy = 0; gy < 8; ++gy) {
            for (unsigned int gx = 0, mask = 0x80; gx < 8; ++gx, mask >>= 1) {
                //printf("%02x & %02x\n", op, mask);
                if (op & mask && gx < 4)
                    set_color(gx, gy, color_red);
                else if (op & mask)
                    set_color(gx, gy, color_blue);
                else
                    set_color(gx, gy, color_black);
            }
        }
    }

    else if (mode_type == 0x02) { // horz red/blue
        for (int gx = 0; gx < 8; ++gx) {
            for (unsigned int gy = 0, mask = 0x80; gy < 8; ++gy, mask >>= 1) {
                if (op & mask && gy < 4)
                    set_color(gx, gy, color_red);
                else if (op & mask)
                    set_color(gx, gy, color_blue);
                else
                    set_color(gx, gy, color_black);
            }
        }
    }
    
    else if (mode_type == 0x03) { // red bitmap
        for (int gy = 0; gy < 8; ++gy) {
            for (unsigned int gx = 0, mask = 0x80; gx < 8; ++gx, mask >>= 1) {
                if (op & mask) {
                    set_color(gx, gy, color_red);
                    //printf("#");
                }
                else {
                    set_color(gx, gy, color_black);
                    //printf(" ");
                }
            }
            //printf("\n");

            if (gy < 7)
                op = program[on_air_ptr++];
        }
        //printf("\n");
    }

    else if (mode_type == 0x04) { // red square, adjust brightness
        set_brightness(BRIGHTNESS * op / 255);

        for (int gy = 0; gy < 8; ++gy) {
            for (int gx = 0; gx < 8; ++gx) {
                set_color(gx, gy, color_red);
            }
        }
    }

    else {
        for (int gy = 0; gy < 8; ++gy) {
            for (int gx = 0; gx < 8; ++gx) {
                set_color(gx, gy, color_red);
            }
        }
    }

    delay(underclock);
}
