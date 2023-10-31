#ifndef _DATABASE_H_
#define _DATABASE_H_

using namespace std;

class CDatabase {
    private:
    char authentication_key;
    char qrcode_key;
    bool notify_flag;
    int new_points;

    //CSensors Sensors;
        
    public:
    CDatabase();
    ~CDatabase();
    bool checkQRCode();
    bool update();
    void sendNotification(bool);
    void sendPoints();

    bool checkLoginCredentials();
    bool checkRegisterCredentials();

};
#endif