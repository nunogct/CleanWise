#ifndef CLOCALSYSTEM_H
#define CLOCALSYSTEM_H

#include <iostream>
#include <pthread.h>
#include <mqueue.h>
#include <memory.h>
#include "CDigitalSensor.h"
#include "CDatabase.h"
#include "CWeightScale.h"
#include "CLed.h"
#include "CMqttConnect.h"

#define INFRARED_SENSOR_PIN 22
#define SWITCH_SENSOR_PIN 16

class CLocalSystem{
private:
CDigitalSensor ss;
CDigitalSensor ir;
CWeightScale ws;
CLed led;
CDatabase db;
CMqttConnect mqtt;

//DATABASE -> BlackBox nota: comunicaçao entre local e base de dados apoiado pelo protocolo mosquito broker
//base dados para o remoto 

pthread_t tUpdateFlags_id;
pthread_t tWeightAcquisition_id;
pthread_t tSendData_id;
pthread_t tFullDumpster_id;
pthread_t tUpdatePoints_id;

pthread_mutex_t mutUpdateFlags;
pthread_mutex_t mutWeightAcquisition;
pthread_mutex_t mutWriteWeight;
pthread_mutex_t mutClientPoints;

pthread_mutex_t mutSendReady;
pthread_cond_t condFull;
pthread_cond_t condSendData;
pthread_cond_t condUpdateFlags;
pthread_cond_t condWeightAcquisition;

pthread_attr_t thread_attr;
struct sched_param thread_param;
int thread_policy;
mqd_t msgqSensors;
struct mq_attr mqAttr;

int ir_sensor;
int ss_sensor;
int send_ready;
int client_id;
int client_points;
int new_points;
bool active_com;

public:
CLocalSystem();
~CLocalSystem();
static void *update_flags(void *arg);
static void *weight_acquisition(void *arg);
static void *send_data(void *arg);
static void *full_dumpster(void *arg);
static void SetupThread(unsigned int prio,pthread_attr_t *pthread_attr,struct sched_param *pthread_param);
void init();
void run();
};

#endif
