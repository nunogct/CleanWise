#include"CLed.h"

CLed::CLed(){

    fd0 = open("/dev/led0", O_WRONLY); //Verificar mais tarde
}

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
