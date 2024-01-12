#include "CDigitalSensor.h"

CDigitalSensor::CDigitalSensor(){}

CDigitalSensor::CDigitalSensor(int pin){
    this->pin = pin;
    status = 0;
}

CDigitalSensor::~CDigitalSensor(){

}

int CDigitalSensor::read(char* msgcontent){
    int temp_pin;
    int temp_sensor; 

    if (sscanf(msgcontent, "%d:%d", &temp_pin, &temp_sensor) == 2) {
        if (temp_pin == pin){
            std::cout << "Valor de " << temp_pin << ": " << temp_sensor << std::endl;
            return temp_sensor;
        }
        return -1;
    }else
       //std::cout << "Error: Reading values!" << std::endl;
       std::cout << "Sem leitura!\n" << std::endl;
    return -2;
}

void CDigitalSensor::setStatus(bool status){
    this->status = status;
}

bool CDigitalSensor::getStatus(){
    return status;

}