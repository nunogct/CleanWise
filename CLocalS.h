#include <pthread.h>

#include "CDatabase.h"
#include "CSensors.h"

#ifndef _CLOCALS_H_
#define _CLOCALS_H_

class CLocalS {
    private:
    CDatabase* db;
    CSensors* sen;

    public:
    CLocalS();
    ~CLocalS();
    void run();
    bool BeginCommunication(CDatabase*); //send request to start communication
    bool CloseCommunication(); //send request to finish communication
    pthread_t tUpdateFlags(void*);
    pthread_t tSendData(void*);
    pthread_t tFullDumpster(void*);
    pthread_t tWeightAcquistiion(void*);

};
#endif