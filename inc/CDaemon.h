#ifndef CHWDAEMON_H
#define CHWDAEMON_H

#include <sys/syslog.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <iostream> 
#include <signal.h>
#include <mqueue.h>
#include <gpiod.h>
#include "CDebounce.h"

#define INFRARED_SENSOR_PIN 22
#define SWITCH_SENSOR_PIN 16
#define MSGQOBJ_NAME    "/msgqSensors"
#define MAX_MSG_LEN     70

class CDaemon{
private:
  CDebounce dbc;

  int ss_pin;
  int ir_pin;

  int ss_val;
  int ir_val;

  int old_ir_val;
  int old_ss_val;

  const char *chipname = "gpiochip0";
  struct gpiod_chip *chip;
  struct gpiod_line *sensor_ir;    
  struct gpiod_line *sensor_switch;
  char *buf = new char[MAX_MSG_LEN + 1];
  mqd_t msgqSensors;
public:
CDaemon();
~CDaemon();
void run();
static void signalHandler(int sig);
void cleanUP();

};
#endif