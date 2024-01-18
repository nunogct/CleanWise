#include "CDaemon.h"

CDaemon::CDaemon() : dbc(10){
    ss_val = 0;
    ir_val = 0;

    int ss_pin = 16;
    int ir_pin = 22;

    int ss_val = 0;
    int ir_val = 0;

    int old_ss_val = 0;
    int old_ir_val = 0;

    chip = gpiod_chip_open_by_name(chipname);
    sensor_ir = gpiod_chip_get_line(chip, ir_pin);
    sensor_switch = gpiod_chip_get_line(chip, ss_pin);

    gpiod_line_request_input(sensor_ir, "example_sensor");
    gpiod_line_request_input(sensor_switch, "example_sensor");
}

CDaemon::~CDaemon(){
    std::cout << "CDaemon Object destructed!" << std::endl;
    exit(EXIT_SUCCESS);
}

void CDaemon::run(){
    signal(SIGHUP, signalHandler); /* catch hangup signal */
    signal(SIGTERM, signalHandler); /* catch kill signal */

    int ss_pin = 16; 
    int ir_pin = 22; 
    int old_ir_val = 0;
    int old_ss_val = 0;

    unsigned int msgprio = 1;
    msgqSensors = mq_open(MSGQOBJ_NAME, O_WRONLY, S_IRWXU | S_IRWXG, NULL);
    char buf[MAX_MSG_LEN + 1];
    while (true) {
        ir_val = gpiod_line_get_value(sensor_ir);
        ss_val = gpiod_line_get_value(sensor_switch);

        if (dbc.debounce()) {
            ir_val = gpiod_line_get_value(sensor_ir);
            if (old_ir_val != ir_val) {
                snprintf(buf, MAX_MSG_LEN, "%d:%d", ir_pin, ir_val);
                mq_send(msgqSensors, buf, MAX_MSG_LEN, 1);
            }
            old_ir_val = ir_val;
        }

        if (dbc.debounce()){
            ss_val = gpiod_line_get_value(sensor_switch);
            if (old_ss_val != ss_val) {
                snprintf(buf, MAX_MSG_LEN, "%d:%d", ss_pin, ss_val);
                mq_send(msgqSensors, buf, MAX_MSG_LEN, 1);
            }
            old_ss_val = ss_val;
        }
    }
}

void CDaemon::cleanUP(){
    gpiod_line_release(sensor_ir);
    gpiod_line_release(sensor_switch);
    gpiod_chip_close(chip);
    mq_close(msgqSensors);
    delete[] buf;
    exit(EXIT_SUCCESS);
    std::cout << "CDaemon Object destructed!" << std::endl;
}

int main(){
    CDaemon daemon;

    pid_t pid, sid;
    pid = fork(); // create a new process

    if (pid < 0) { // on error exit
        syslog(LOG_ERR, "%s\n", "fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        std::cout << "Deamon PID: " << pid << std::endl;
        exit(EXIT_SUCCESS); // parent process (exit)
    }
    sid = setsid(); // create a new session

    if (sid < 0) { // on error exit
        syslog(LOG_ERR, "%s\n", "setsid");
        exit(EXIT_FAILURE);
    }
    // make '/' the root directory
    if (chdir("/") < 0) { // on error exit
        syslog(LOG_ERR, "%s\n", "chdir");
        exit(EXIT_FAILURE);
    }
    umask(0);

    close(STDIN_FILENO);  // close standard input file descriptor
    close(STDOUT_FILENO); // close standard output file descriptor
    close(STDERR_FILENO); // close standard error file descriptor

    daemon.run();
}
