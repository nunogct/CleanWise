#ifndef CDEBOUNCE_H
#define CDEBOUNCE_H 

#include <ctime>
class CDebounce {
private:
    clock_t last_trigger;
    int debounce_time;

public:
    CDebounce(int debounce_time);
    ~CDebounce();
    bool debounce();
};

#endif