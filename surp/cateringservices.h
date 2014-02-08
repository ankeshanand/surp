#ifndef CATERINGSERVICES_H
#define CATERINGSERVICES_H


#include <QPair>
#include <QSqlDatabase>
#include "databasemanager.h"

/*We use a single CateringServices object throughout the program. Once the customer has been catered,
database updated, data has to be reinitialised. */

class CateringServices
{
private:
    QString CustomerToken;
    QSqlDatabase db;
    bool findCustomer();
    DatabaseManager *dbm;

public:
    CateringServices();
    bool updateBill(const QString& tokenNumber, const QList<QPair<QString, int> >& orderList, int orderBill);
    void reinitialiseData();
};

#endif // CATERINGSERVICES_H
