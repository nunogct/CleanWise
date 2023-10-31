#include<string>

#ifndef _CADMIN_H_
#define _CADMIN_H_

using namespace std;

struct ItemCatalog{
    int id;
    string name;
    int price;
};

class CAdmin {
    private:
    

    public:
    CAdmin();
    ~CAdmin();
    bool addDumpster();
    bool editDumpster(int,string,int);
    bool removeDumpster(int);

    bool editUser(int,string,string,string,int);
    bool removeUser(int);
    
    bool addItemCatalog(string,int);
    bool removeItemCatalog();

};
#endif