#include "management.h"
#include <QDebug>

Management::Management()
{
    dbm = DatabaseManager::Instance();
    int i;

    for(i=0;i<12;i++)
        AverageOccupancy[i] = 0;

    i=0;
    QSqlQuery query1("SELECT Average_Occupancy FROM Management");

    while(query1.next())
    {
        AverageOccupancy[i] = query1.value(0).toFloat();
        i++;
    }

    QSqlQuery query2("SELECT Rates FROM MANAGEMENT");
    i = 0;
    while(query2.next())
    {
        if(i>3)
            continue;
        Rates[i] = query2.value(0).toString().toFloat();
        i++;
    }
}

float Management::getAverageOccupancy(int i)
{
    return AverageOccupancy[i];
}

void Management::updateTariff(const float& percent, const int& change )
{
    int i;
    for(i = 0; i<4; i++)
    {
        /* keeping track of previous rates, to be used for database update */
        P_Rates[i] = Rates[i];
        Rates[i] = (change==0)?(Rates[i]+Rates[i]*percent/100.0):(Rates[i]-Rates[i]*percent/100.0);
        qDebug()<<Rates[i]<<" "<<P_Rates[i];
    }
    for(i=0;i<4;i++)
    {
        QSqlQuery query1("UPDATE Management SET Rates = "+QString::number(Rates[i])+" WHERE Rates = "+QString::number(P_Rates[i]));
    }



}

bool Management::updateThreshold(int threshold)
{
    QSqlQuery query1("UPDATE Management SET Threshold_Visit="+QString::number(threshold)+" WHERE Threshold_Visit!=0");
    if(query1.isActive())
        return true;
    else
        return false;
}
