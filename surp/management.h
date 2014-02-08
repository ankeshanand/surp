#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <iostream>
#include "databasemanager.h"
using namespace std;

class Management
{
private:
    float AverageOccupancy[12];
    float Rates[4];
    float P_Rates[4];
    float Threshold;
    DatabaseManager *dbm;
public:
    float CurrentOccupancy;
    Management();
    float getAverageOccupancy(int i);
    void updateTariff(const float&,const int&);
    bool updateThreshold(int threshold);
    void increaseOccupancy();

};

#endif // MANAGEMENT_H
