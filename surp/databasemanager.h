#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql>
#include <QtGui>

class DatabaseManager
{
    friend class Reservation;
    friend class CheckIn;
    friend class CateringServices;
    friend class CheckOut;
public:
    static DatabaseManager* Instance();
    friend bool createConnection();
protected:
    DatabaseManager();
private:
    static DatabaseManager* _instance;
    QSqlDatabase db;
};

#endif // DATABASEMANGER_H
