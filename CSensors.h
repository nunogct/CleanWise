#include "CDatabase.h"

#ifndef _CSENSORS_H_
#define _CSENSORS_H_

class CSensors {
    private:
    bool flag_ir;
    bool flag_reed_switch;
    bool flag_weigh_scale;
    bool flag_scan;

    public:
    CSensors();
    ~CSensors();
    bool getIR(bool);
    bool getReedSwitch(bool);
    bool getWeightScale(bool);
    bool getScan(bool);

};
#endif