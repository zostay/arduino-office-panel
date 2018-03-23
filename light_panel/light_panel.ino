#include "grid.h"
#include "arduino-grid.h"
#include "piles.h"
#include "on_air.h"

#define PILE_GRID      1
#define ON_AIR_GRID    2
#define EMERGENCY_GRID 3

#define PIN_GRID       2
#define PIN_ON_AIR     4
#define PIN_QUIET      5

#define EMERGENCY_DURATION  500

Grid *current;
Panel *panel;

void setup() {
    Serial.begin(115200);

    pinMode(PIN_GRID, OUTPUT);
    pinMode(PIN_ON_AIR, INPUT);
    pinMode(PIN_QUIET, INPUT);

    panel = new ArduinoPanel();
    
    current = new PileGrid(panel);
}

int mode = PILE_GRID, last_mode = PILE_GRID;
//int emergency_cycles = 0;
void loop() {
    int on_air = digitalRead(PIN_ON_AIR);
    int emergency = digitalRead(PIN_QUIET);

    if (on_air == LOW) {
        //emergency_cycles = 0;
        mode = PILE_GRID;
        //digitalWrite(PIN_EMERGENCY_BLINK, LOW);
    }
    else {
        //if (mode == EMERGENCY_GRID) {
            //if (emergency_cycles-- <= 0) {
                //digitalWrite(PIN_EMERGENCY_BLINK, LOW);
                mode = ON_AIR_GRID;
            //}
        //}
        //else {
            //emergency_cycles = 0;

            mode = ON_AIR_GRID;

            //if (mode == ON_AIR_GRID) {
                if (emergency == HIGH) {
                    mode = EMERGENCY_GRID;
                    //emergency_cycles = EMERGENCY_DURATION;
                    //digitalWrite(PIN_EMERGENCY_BLINK, HIGH);
                }
            //}
        //}
    }

    if (last_mode != mode) {
        last_mode = mode;
        delete current;

        switch (mode) {
            case PILE_GRID:
                current = new PileGrid(panel);
                break;
            case ON_AIR_GRID:
                current = new OnAirGrid(panel, ON_AIR_PROGRAM);
                break;
            case EMERGENCY_GRID:
                current = new OnAirGrid(panel, EMERGENCY_PROGRAM);
                break;
        }
    }

    current->start_loop();
    current->loop();
    current->finish_loop();

//    if (count-- <= 0) {
//        delete current;
//        if (mode == 0) {
//            current = new OnAirGrid();
//            mode = 1;
//        }
//        else {
//            current = new PileGrid();
//            mode = 0;
//        }
//        count = 100;
//    }

    //delay(10);
}
