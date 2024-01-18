#include "CDatabase.h"

CDatabase::CDatabase(){}

CDatabase::~CDatabase(){
    std::cout << "CDatabase Object destructed with success!" << std::endl;
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
}

int CDatabase::initConnection(){
    int vc;
    my_ssh_session = ssh_new();
    if(my_ssh_session == nullptr){
        throw std::runtime_error("Error initializing SSH session.");
    }
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, user.c_str());

    vc = ssh_connect(my_ssh_session);
    if(vc != SSH_OK){
        std::cerr << "Error: Connecting to the SSH server.\n" << std::endl;
        return(-1);
    }

    vc = ssh_userauth_password(my_ssh_session, nullptr, password.c_str());
    if(vc != SSH_AUTH_SUCCESS){
        std::cerr << "Error: Password authentication failed.\n" << std::endl;
        return(-2);
    }
    return 0;
}

int CDatabase::closeConnection(){
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    return 0;
}

int CDatabase::openChannel(){
    int vc;
    chnn = ssh_channel_new(my_ssh_session);

    if(chnn == nullptr){
        std::cerr << "Erro: SSH channel not created!\n" << std::endl;
        return(-3);
    }

    vc = ssh_channel_open_session(chnn);
    if(vc != SSH_OK){
        std::cerr << "Error: SSH session not opened!\n" << std::endl;
        return(-4);
    }
    return 0;
}

int CDatabase::closeChannel(){
    ssh_channel_send_eof(chnn);
    ssh_channel_free(chnn);
    return 0;
}

int CDatabase::getClientPoints(int client_id){
    int result;

    openChannel();

    std::string command = mysql_access + " -e \"SELECT PONTOS FROM Clientes WHERE CLIENTE_ID = " + std::to_string(client_id) + ";\"";
    int vc = ssh_channel_request_exec(chnn, command.c_str());

    if(vc != SSH_OK){
        std::cerr << "Error: Remote command not executed!" << std::endl;
        closeChannel();
        return 0; // or any default value
    }

    char buf[16];
    vc = ssh_channel_read(chnn, buf, sizeof(buf), 0);

    closeChannel();

    if(vc != SSH_OK){
        std::cerr << "Error: Reading channel failed!" << std::endl;
        return 0; // or any default value
    }

    std::istringstream stream(buf);

    if(!(stream >> result)){
        std::cerr << "Error: Failed to convert string to integer." << std::endl;
        return 0;
    }

    return result;
}

int CDatabase::setContainerState(int state){
    int vc;
    openChannel();
    std::string command = mysql_access + " -e \"UPDATE Contentores SET STATUS = " + std::to_string(state) + " WHERE CONTENTOR_ID = " + container_id + ";\"";
    vc = ssh_channel_request_exec(chnn, command.c_str());

    if(vc != SSH_OK){
        std::cerr << "Error: Remote command not executed!\n" << std::endl;
        exit(-5);
    }

    closeChannel();
    return 0;
}

int CDatabase::setClientPoints(int client_id, int points){
    int vc;
    openChannel();

    std::string command = mysql_access + " -e \"UPDATE Clientes SET PONTOS = " + std::to_string(points) + " WHERE CLIENTE_ID = " + std::to_string(client_id) + ";\"";
    vc = ssh_channel_request_exec(chnn, command.c_str());

    if(vc != SSH_OK){
        std::cerr << "Error: Remote command not executed!\n" << std::endl;
        return(-5);
    }

    closeChannel();
    return 0;
}
