#include "CMqttConnect.h"

CMqttConnect::CMqttConnect() {
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, &received_message);
    
        if (!mosq) {
            fprintf(stderr, "Error creating Mosquitto client.\n");
            exit(-3);
        }

    mosquitto_connect_callback_set(mosq, on_connect_callback);
    mosquitto_message_callback_set(mosq, on_message_callback);
    mosquitto_user_data_set(mosq, this);

    received_message = nullptr;
    int var = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 10);  
    if (var != MOSQ_ERR_SUCCESS) {
        fprintf(stderr,"Error connecting to server: %s\n", mosquitto_strerror(var));
        exit(-5);
    }

    std::cout << "CMqttConnect Object created with success!" << std::endl;
}

CMqttConnect::~CMqttConnect(){
    free(received_message);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    std::cout << "CMqttConnect Object destructed with success!" << std::endl;
}

void CMqttConnect::init(){
      mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, &received_message);
    
        if (!mosq) {
            fprintf(stderr, "Error creating Mosquitto client.\n");
            exit(-3);
        }

    mosquitto_connect_callback_set(mosq, on_connect_callback);
    mosquitto_message_callback_set(mosq, on_message_callback);
    mosquitto_user_data_set(mosq, this);

    received_message = nullptr;
    int var = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 20);  
    if (var != MOSQ_ERR_SUCCESS) {
        fprintf(stderr,"Error connecting to server: %s\n", mosquitto_strerror(var));
        exit(-5);
    }
}

void CMqttConnect::on_connect_callback(struct mosquitto *mosq, void *obj, int rc) {
    static_cast<CMqttConnect *>(obj)->on_connect(mosq, rc);
}

void CMqttConnect::on_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    static_cast<CMqttConnect *>(userdata)->on_message(msg);
}

void CMqttConnect::on_connect(struct mosquitto *mosq, int rc) {
    if(rc == 0){
        std::cout << "Successfully connected to: " <<  BROKER_ADDRESS << std::endl;
        mosquitto_subscribe(mosq, NULL, TOPIC, 0);
    }else{

        //fprintf(stderr, "Error connecting to server: %s\n", mosquitto_strerror(rc));
        std::cout << "Error connecting to server: " <<  mosquitto_strerror(rc) << std::endl;
        exit(-4);
    }
}

void CMqttConnect::on_message(const struct mosquitto_message *msg) {
    free(received_message);
    if(msg->payload != NULL){
            received_message = strdup((char *)msg->payload);
        //char *endptr;
        //long value = strtol((char *)msg->payload, &endptr, 10);
    }
}

int CMqttConnect::receiveMessage() {
    int var;

        var = mosquitto_loop(mosq, 1000, 1);  // Limit the wait time to 100 ms
        if (var != MOSQ_ERR_SUCCESS) {
            std::cout << "Error in the loop: " << mosquitto_strerror(var) << std::endl;
            return -1;
        }/*else{

        std::cout << "MQTT not connected." << std::endl;
        }*/

        if (received_message != NULL){
            client_id = atoi(received_message);
            received_message = nullptr;
            return 1;
        }
        return 0;
}

int CMqttConnect::getClientID(){
    return client_id;
}

int CMqttConnect::publish_Message()
{
    const char* payload = "1";

    int verification = mosquitto_publish(mosq, NULL, "dumpster", strlen(payload), payload, 1, false);

    if (verification != MOSQ_ERR_SUCCESS) 
        return false;
    else 
        return true;

}

/*int main(){
    CMqttConnect m;
    while(m.receiveMessage() != 1){}
    std::cout << "CLIENT_ID: " << m.getClientID() << std::endl;
    printf("%d\n",m.receiveMessage());
}*/