#include"CLed.h"

CLed::CLed(){

    //system("insmod led.ko");
    fd0 = open("/dev/led0", O_WRONLY);
    //system("lsmod");
    system("echo none >/sys/class/leds/led0/trigger");
}
//    system("insmod led.ko");

CLed::~CLed(){
    close(fd0);
    putchar('\n');
    system("rmmod led");
}

void CLed::changeColor(char state){
    write(fd0, &state, 1);
}

/*int main(){
    CLed led;
    char state;
    led.changeColor('1');
    sleep(1);
    led.changeColor('2');;
    sleep(1);
    led.changeColor('3');;
    sleep(1);
}*/
