#ifndef CHECKIN_H
#define CHECKIN_H

#include "databasemanager.h"

#include <sstream>
using namespace std;

class CheckIn
{
private:
    string CustomerToken;
    float AdvancePaid;
    string name;

public:
    CheckIn(string , float );
    bool identifyReservation();
    string issueId();
    void updateAllCustomerDatabase();
};

#endif // CHECKIN_H
