#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

#ifndef CHWDAEMON_H
#define CHWDAEMON_H

class CLed{
private:
int fd0;
public:
CLed();
~CLed();
void changeColor(char);

};
#endif

/*
    int fd0 = open("/dev/led0", O_WRONLY);
    char ledOn = '1', ledOff = '0';
    system("echo none >/sys/class/leds/led0/trigger");
    printf("\nStarting loop (5 times):");
    sleep(1);
    while(count != 5)
    {
        
        write(fd0, &ledOn, 1);
        printf("\non!");
        sleep(4);
       
        write(fd0, &ledOff, 1);
        printf("\noff!");
        sleep(4);
        count++;
    }
    printf("Closing Device Driver.\n");
    close(fd0);
    */