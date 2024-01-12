#include "CDebounce.h"

CDebounce::CDebounce(int debounce_time) : last_trigger(clock()), debounce_time(debounce_time) {}

CDebounce::~CDebounce() {}

bool CDebounce::debounce() {
    clock_t currentTime = clock();
    int elapsedTime = static_cast<int>(1000.0 * (currentTime - last_trigger) / CLOCKS_PER_SEC);

    if (elapsedTime >= debounce_time) {
        last_trigger = currentTime;
        return true;  
    }

    return false;  
}
