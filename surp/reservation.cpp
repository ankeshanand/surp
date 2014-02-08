#include "reservation.h"


Reservation::Reservation(string name,string checkinDate ,string checkoutDate,int numberOfRooms,Room* rooms)
{   CustomerName = name;
    NumberOfRooms = numberOfRooms;
    RequiredRooms = new Room[NumberOfRooms];
    RequiredRooms = rooms;
    CheckInDate = checkinDate;
    CheckOutDate = checkoutDate;

    if(CheckAvailability()){    //if available add customer in residingcustomer database
        addCustomer();
    }
    else generateApology();     //else generate appology
}
bool Reservation::CheckAvailability(){
    DatabaseManager *dbm = DatabaseManager::Instance();
    // try to open database

    if(!dbm->db.open()){
        QMessageBox::critical(0, QObject::tr("Database Error"),dbm->db.lastError().text());
    }

    // if opened
    else{               // selecting only those rooms which are not available during this time
        QSqlQuery query("SELECT Rooms FROM `Residing_Customer` WHERE ( CheckIn_Date <= '"+QString::fromStdString(CheckInDate)+"' AND CheckOut_Date >= '"+QString::fromStdString(CheckInDate)+"') OR ( CheckIn_Date <= '"+QString::fromStdString(CheckOutDate)+"' AND CheckOut_Date >= '"+QString::fromStdString(CheckOutDate)+"')",dbm->db);
        if(!query.isActive()){
            QMessageBox::critical(0, QObject::tr("Query check Error"),query.lastError().text());
        }

        QList<int> unavailableRooms;
        while(query.next()){
            QStringList list = query.value(query.record().indexOf("Rooms")).toString().split(";");
             while(!list.isEmpty()){
                 unavailableRooms.push_back(list.front().toInt());   //putting those room in vector of unavailable rooms
                 list.pop_front();
             }
        }
        for(int i=0;i<NumberOfRooms;i++){       // iterating through list of rooms where room are type of required room
            QSqlQuery query1("SELECT room_no FROM Rooms WHERE room_no LIKE '"+QString::number(RequiredRooms[i].RoomType)+"%'",dbm->db);
            if(!query1.isActive()){
                QMessageBox::critical(0, QObject::tr("Query room allot Error"),query1.lastError().text());
            }

            while(query1.next()){
                int roomno = query1.value(0).toInt();
                if(!unavailableRooms.contains(roomno)){
                    RequiredRooms[i].RoomNumber = roomno;               //if current room is not in unavailable list
                    RequiredRooms[i].Status = "Available";              //then allocate room no to required rooms
                    unavailableRooms.push_back(roomno);                // and add it to unavailable list
                    break;
                }
            }
            if(RequiredRooms[i].Status.compare("Available")){    //if room is not available return false
                return false;
            }
        }
    }
    return true;
}

// generating Apology
void Reservation::generateApology(){
    QMessageBox::information(0, QObject::tr("No Vacancy"),"Sorry,but there is no space available");
}

// adding corresponding customer to database of residing customer
void Reservation::addCustomer(){

    DatabaseManager *dbm = DatabaseManager::Instance();
    if (!dbm->db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),dbm->db.lastError().text());
    }
   int token = allotCustomerToken();
   string roomstring = allotRooms();
   QSqlQuery query("INSERT INTO `Residing_Customer` VALUES('"+QString::fromStdString(CustomerName)+"',"+QString::number(token)+",date('"+QString::fromStdString(CheckInDate)+"'),date('"+QString::fromStdString(CheckOutDate)+"'),"+QString::number(NumberOfRooms)+",'"+QString::fromStdString(roomstring)+"',NULL,NULL,NULL)",dbm->db);
    if(!query.isActive()){
        QMessageBox::critical(0, QObject::tr("Query addcustomer allot Error"),query.lastError().text());
    }
    else QMessageBox::information(0,QObject::tr("ALLOTED ROOMS AND TOKEN"),"Alloted Rooms :"+QString::fromStdString(roomstring)+"\n Alloted Customer Token :"+QString::number(token));
}

 //alloting 5 digit customer Token randomly
int Reservation::allotCustomerToken(){
    srand((unsigned int)time(NULL));
    return rand()%100000+10000;
}

//converting roomnumber of alloted rooms to string
string Reservation::allotRooms(){
    string roomstring="";
    for(int i=0;i<NumberOfRooms;i++){
        ostringstream s;
        s << RequiredRooms[i].RoomNumber;
        roomstring += s.str();          // converting number to string using stringstream
        if(i!=NumberOfRooms-1)
            roomstring +=";";
    }
    return roomstring;          //return string containing roomno of alloted rooms
}
