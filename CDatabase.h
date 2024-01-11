#include <iostream>
#include <string>
#include <sstream>
#include <libssh/libssh.h>

#ifndef CDATABASE_H
#define CDATABASE_H

class CDatabase{

private:
ssh_session my_ssh_session;
ssh_channel chnn;

std::string host = "159.65.48.110";  
std::string user = "luiz";           
std::string password = "cleanwise123";

std::string db_name = "CleanWise";
std::string db_user = "LuizAdmin";
std::string db_port = "3306";

std::string container_id = "1";
int client_points = 0;

std::string mysql_access = "mysql -h " + host + " -P " + db_port + " -u " + db_user +  " -p" + password + " " + db_name;

int openChannel();
int closeChannel();

public:
CDatabase();
~CDatabase();
int initConnection();
int setContainerState(int state);
int getClientPoints(int client_id);
int setClientPoints(int client_id, int points);
int closeConnection();
};
#endif