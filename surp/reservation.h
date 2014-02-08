#ifndef RESERVATION_H
#define RESERVATION_H
#include "room.h"
#include <sstream>
#include <QtSql>
#include "databasemanager.h"
#include <QMessageBox>

using namespace std;

class Room;


class Reservation
{

private :
    string CustomerName;
    string CheckInDate;
    string CheckOutDate;
    int NumberOfRooms;
    Room* RequiredRooms;
public:
    Reservation(string,string,string,int,Room*);
    bool CheckAvailability();
    void addCustomer();
    void generateApology();
    int allotCustomerToken();
    string allotRooms();
    void updateCurrentOccupancy();
};

#endif // RESERVATION_H
