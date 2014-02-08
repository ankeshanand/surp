#ifndef ROOM_H
#define ROOM_H
#include <string>
#include "mainwindow.h"
#include "reservation.h"

using namespace std;

class Room
{   friend class ResidingCustomer;
    friend class MainWindow;
    friend class Reservation;
private:
    int RoomNumber;
    string Status;
    int RoomType;
    float Rate;
public:
    Room();
    void changeRoomStatus();
};

#endif // ROOM_H
