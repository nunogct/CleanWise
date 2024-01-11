#ifndef CWEIGHTSCALE_H
#define CWEIGHTSCALE_H

#define LOADCELL_DOUT_PIN 23
#define LOADCELL_SCK_PIN 24
#include <gpiod.h>
#include <iostream>
#include <unistd.h>

class CWeightScale {
private:
    bool status;
    int new_weight;
    int total_weight;
    int reading_value;

    const char *chipname = "gpiochip0";
struct HX711 {
    struct gpiod_chip *chip;
    struct gpiod_line *pd_sck;
    struct gpiod_line *dout;
    float GAIN;
    long OFFSET;
    int SCALE;
} *hx;

bool HX711_is_ready();  

public:
    CWeightScale();
    ~CWeightScale();
    void run();
    int convertValue();
    int getNewWeight();
    void saveTotalWeight();
    int getTotalWeight();
    void setStatus(bool status);
    bool getStatus();
};

#endif