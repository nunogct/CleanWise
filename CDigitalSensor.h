#ifndef CDIGITALSENSOR_H
#define CDIGITALSENSOR_H 

#include <gpiod.h>
#include <iostream>

#define MAX_LEN 10000
class CDigitalSensor{
    
private:
char msgcontent[MAX_LEN];
int pin;
bool status;

public:
CDigitalSensor();
CDigitalSensor(int pin);
~CDigitalSensor();

int read(char* msgcontent);
void setStatus(bool);
bool getStatus();
};

#endif