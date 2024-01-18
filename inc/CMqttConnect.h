#ifndef CMQTTCONNECT_H
#define CMQTTCONNECT_H

//#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <errno.h>
#include <mosquitto.h>

class   CMqttConnect {
private:
const char *BROKER_ADDRESS = "cleanwise.cloud";
const int BROKER_PORT = 1883;
const char *TOPIC = "CONTAINER1";
struct mosquitto *mosq;
int rc;
char *received_message;
int client_id;

void on_connect(struct mosquitto *mosq, int rc);
void on_message(const struct mosquitto_message *msg);

public:
CMqttConnect();
~CMqttConnect();
void init();
static void on_connect_callback(struct mosquitto *mosq, void *obj, int rc);
static void on_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg);
int receiveMessage();
int getClientID();
int publish_Message();
};

#endif
