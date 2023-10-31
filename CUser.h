#include<string>

#ifndef _CUSER_H_
#define _CUSER_H_

using namespace std;

class CUser {
    private:
    int user_id;
    string name;
    string email;
    string password;
    int phone_number;
    int score;

    public:
    CUser();
    ~CUser();
    void registerUser();
    void loginUser();
    void scanQRCode();
    void editProfile();
    bool purchaseItem();
    int getScore();

};
#endif