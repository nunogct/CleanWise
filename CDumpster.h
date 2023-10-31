#include<string>

#ifndef _CDUMPSTER_H_
#define _CDUMPSTER_H_

using namespace std;

class CDumpster {
    private:
    int id;
    int coordinates;
    string type;
    bool flag_avaiable;

    public:
    CDumpster(int, string);
    ~CDumpster();
    // setType(string);
    


};
#endif