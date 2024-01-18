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