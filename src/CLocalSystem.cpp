#include "CLocalSystem.h"
#define MSGQOBJ_NAME "/msgqSensors"

#define MAX_LEN 10000

CLocalSystem::CLocalSystem() : mqtt(), ss(SWITCH_SENSOR_PIN), ir(INFRARED_SENSOR_PIN), db(), ws(), led(){   
    db.initConnection();
    ir_sensor = 0;
    ss_sensor = 1;
    new_points = 0;
    client_points = 0;
    active_com = false;
    send_ready = 0;

    mqtt.init();
    ss.setStatus(1);
    ir.setStatus(0);
    ws.setStatus(0);
    led.changeColor('1');
}

CLocalSystem::~CLocalSystem(){
    pthread_mutex_destroy(&mutUpdateFlags);
    pthread_mutex_destroy(&mutWeightAcquisition);
    pthread_mutex_destroy(&mutClientPoints);

    pthread_attr_destroy(&thread_attr);
    mq_close(msgqSensors);
    db.closeConnection();
}

void CLocalSystem::init(){
    pthread_mutex_init(&mutUpdateFlags, NULL);
    pthread_mutex_init(&mutWeightAcquisition, NULL);
    pthread_mutex_init(&mutClientPoints, NULL);

    pthread_attr_init(&thread_attr);
    pthread_attr_getschedpolicy(&thread_attr, &thread_policy);
    pthread_attr_getschedparam(&thread_attr, &thread_param);

    SetupThread(50, &thread_attr, &thread_param);
    pthread_create(&tWeightAcquisition_id, &thread_attr, weight_acquisition, this);

    SetupThread(65, &thread_attr, &thread_param);
    pthread_create(&tUpdateFlags_id, &thread_attr, update_flags, this);

    SetupThread(55, &thread_attr, &thread_param);
    pthread_create(&tSendData_id, &thread_attr, send_data, this);

    SetupThread(45, &thread_attr, &thread_param);
    pthread_create(&tFullDumpster_id, &thread_attr, full_dumpster, this);

    msgqSensors = mq_open(MSGQOBJ_NAME, O_RDWR, 0666, NULL);
    if (msgqSensors == (mqd_t)-1){
        perror("CLocalS(): Opening message queue\n");
    }

    mq_getattr(msgqSensors, &mqAttr);
    mqAttr.mq_flags = O_NONBLOCK;
    mq_setattr(msgqSensors, &mqAttr, nullptr);
}

void CLocalSystem::run(){
    pthread_join(tWeightAcquisition_id, NULL);
    pthread_join(tUpdateFlags_id, NULL);
    pthread_join(tSendData_id, NULL);    
    pthread_join(tFullDumpster_id, NULL);
}

void CLocalSystem::SetupThread(unsigned int prio, pthread_attr_t *pthread_attr, struct sched_param *pthread_param){
    int rr_min_priority, rr_max_priority;

    rr_min_priority = sched_get_priority_min(SCHED_RR);
    rr_max_priority = sched_get_priority_max(SCHED_RR);
    pthread_param->sched_priority = prio; 

    pthread_attr_setschedparam(pthread_attr, pthread_param);
    pthread_attr_setschedpolicy(pthread_attr, SCHED_RR);
}

void *CLocalSystem::update_flags(void *arg){
    CLocalSystem *obj = reinterpret_cast<CLocalSystem*>(arg);
    char msgcontent[MAX_LEN];

    while (1){
        pthread_mutex_lock(&obj->mutUpdateFlags);
        if(mq_receive(obj->msgqSensors, msgcontent, MAX_LEN, NULL) == -1){
        } else {
            if(obj->ir.read(msgcontent) != -1)
                obj->ir_sensor = obj->ir.read(msgcontent);
            
            if(obj->ss.read(msgcontent) != -1)
                obj->ss_sensor = obj->ss.read(msgcontent);
        }

        pthread_mutex_unlock(&obj->mutUpdateFlags);  
    }
}

void *CLocalSystem::weight_acquisition(void *arg){
    CLocalSystem *obj = static_cast<CLocalSystem*>(arg);
    while(1){
        pthread_mutex_lock(&obj->mutWeightAcquisition);
        if(obj->active_com == false && obj->mqtt.receiveMessage()){
            obj->client_id = obj->mqtt.getClientID();
            obj->active_com = true;
            obj->client_points = obj->db.getClientPoints(obj->client_id);  
        }
        pthread_mutex_unlock(&obj->mutWeightAcquisition);       

        pthread_mutex_lock(&obj->mutUpdateFlags);
        if(obj->ss_sensor == 0 && obj->ss.getStatus()){
            obj->ws.run();
            obj->ws.setStatus(1);
            obj->ir.setStatus(0);
            obj->led.changeColor('3');
            obj->send_ready = 1;
        }
        pthread_mutex_unlock(&obj->mutUpdateFlags);
    }
}

void *CLocalSystem::send_data(void *arg){
    CLocalSystem *obj = static_cast<CLocalSystem*>(arg);
    while(1){
        pthread_mutex_lock(&obj->mutUpdateFlags);
        if(obj->ss_sensor == 1 && obj->ss.getStatus() && obj->send_ready == 1){   
            obj->db.setClientPoints(obj->client_id, obj->new_points);
            obj->send_ready = 0;
            obj->ir.setStatus(1);
        }
        pthread_mutex_unlock(&obj->mutUpdateFlags);

        pthread_mutex_lock(&obj->mutWeightAcquisition);
        if(obj->active_com){
            pthread_mutex_lock(&obj->mutClientPoints);
            obj->new_points = obj->client_points + obj->ws.getNewWeight();
            pthread_mutex_unlock(&obj->mutClientPoints);
        }
        pthread_mutex_unlock(&obj->mutWeightAcquisition);
    }
}

void *CLocalSystem::full_dumpster(void *arg) {
    CLocalSystem *obj = static_cast<CLocalSystem*>(arg);
    
    while(1){     
        pthread_mutex_lock(&obj->mutUpdateFlags);
        if(obj->ir_sensor == 1 && obj->ir.getStatus()){   
            obj->db.setContainerState(1);
            obj->led.changeColor('2');
            int verification = obj->mqtt.publish_Message();
            obj->ss.setStatus(0);
        }
        pthread_mutex_unlock(&obj->mutUpdateFlags);
    }
}

int main(){
    CLocalSystem cls;
    cls.init();
    cls.run();
}
