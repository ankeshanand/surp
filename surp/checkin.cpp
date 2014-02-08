#include "checkin.h"

CheckIn::CheckIn(string token,float advance)
{   // assigning customer token to datamembers
    CustomerToken = token;
    AdvancePaid = advance;

    // cheking if reservation exists or not
    if(!identifyReservation())
        QMessageBox::critical(0, QObject::tr("Check In"),"Reservation Does Not Exist");
    // reservation does not exist
    else updateAllCustomerDatabase();
}

bool CheckIn::identifyReservation(){
    //create database connection
    DatabaseManager *dbm = DatabaseManager::Instance();
    //query fro checking if customer with corresponding token exist in database or not
    QSqlQuery query("SELECT * FROM `Residing_Customer` WHERE Customer_Token = '"+QString::fromStdString(CustomerToken)+"'",dbm->db);
    if(!query.isActive())
            QMessageBox::critical(0, QObject::tr("Identify Reservation"),query.lastError().text());
    query.exec();
    if(!query.next())
        return false;   //if not return false
    //update name as datamember of customer if exist

    name = query.value(query.record().indexOf("Name")).toString().toStdString();
    QDate checkindate = query.value(query.record().indexOf("CheckIn_Date")).toDate();
    QDate checkoutdate = query.value(query.record().indexOf("Checkout_Date")).toDate();
    QMessageBox::information(0,QObject::tr("Check In"),"Name : "+QString::fromStdString(name)+" Alloted Rooms : "+query.value(query.record().indexOf("Rooms")).toString());
    QStringList rooms = query.value(query.record().indexOf("Rooms")).toString().split(";");
    QSqlQuery getrates_query("SELECT Rates FROM `Management`",dbm->db);
    float bill = 0,Rates[4];
    int i = 0;
    while(getrates_query.next() && i<4)
        Rates[i++] = getrates_query.value(0).toFloat();

    i = 0;
    while(i<rooms.size())
        bill += Rates[rooms.at(i++).toInt()/100]*(checkoutdate.toJulianDay()-checkindate.toJulianDay());


    // query for updateing advance of the customer
    QSqlQuery update_query("UPDATE `Residing_Customer` SET Advance = "+QString::number(AdvancePaid)+" ,Bill = "+QString::number(bill)+" WHERE Customer_Token = '"+QString::fromStdString(CustomerToken)+"'",dbm->db);
    if(!update_query.isActive())
        QMessageBox::critical(0, QObject::tr("Identify Reservation"),update_query.lastError().text());
    return true;
}

// issue id to frequent customer
string CheckIn::issueId(){

    srand((unsigned int)time(NULL));
    ostringstream s;
    s << rand()%10000+1000;
    return "I"+s.str();
}

// updating all customer database
void CheckIn::updateAllCustomerDatabase(){

    DatabaseManager *dbm = DatabaseManager::Instance();
    // check if customer exist in all customer database or not
    QSqlQuery query("SELECT * FROM `All_Customer` WHERE Name = '"+QString::fromStdString(name)+"'",dbm->db);
    if(!query.next()){
        //if does not exist then insert it in table
        QSqlQuery insert_query("INSERT INTO `All_Customer` VALUES('"+QString::fromStdString(name)+"',1,0,NULL)",dbm->db);
        if(!insert_query.isActive())
            QMessageBox::critical(0, QObject::tr("Update All Customer Database"),insert_query.lastError().text());
    }
    // if does not exist
    else{
        // query for updating visits of existing customer
        QSqlQuery update_query("UPDATE `All_Customer` SET Visits = Visits + 1 WHERE Name = '"+QString::fromStdString(name)+"'",dbm->db);
        if(!update_query.isActive())
            QMessageBox::critical(0, QObject::tr("Update All Customer Database"),update_query.lastError().text());
        //query for fetching threhold visit to issue id
        QSqlQuery check_query("SELECT Threshold_Visit  FROM `Management` ",dbm->db);
        if(!check_query.isActive())
            QMessageBox::critical(0, QObject::tr("Update All Customer Database"),check_query.lastError().text());
        //if query is active and threshold exists
        else if(check_query.next()){
            //query for checking the frequency of customer
            QSqlQuery compare_query("SELECT * FROM `All_Customer` WHERE Name= '"+QString::fromStdString(name)+"' AND Visit_MonthCount != 0 AND Visits/Visit_MonthCount >= "+check_query.value(check_query.record().indexOf("Threshold_Visit")).toString(),dbm->db);
            if(!compare_query.isActive())
                QMessageBox::critical(0, QObject::tr("Update All Customer Database"),compare_query.lastError().text());
            // if it is fair enough than issue Id
            else if(!compare_query.isNull(0)){
                //query for updating customer id
                QSqlQuery updateid_query("UPDATE `All_Customer` SET Customer_Id ="+QString::fromStdString(issueId())+" WHERE Name = '"+QString::fromStdString(name)+"'",dbm->db);
                if(!updateid_query.isActive())
                    QMessageBox::critical(0, QObject::tr("Update All Customer Database"),updateid_query.lastError().text());
            }
            // else remove id if exist
            else{
                //updating id as NULL
                QSqlQuery updateid_query("UPDATE `All_Customer` SET Customer_Id = NULL WHERE Name = '"+QString::fromStdString(name)+"'",dbm->db);
                if(!updateid_query.isActive())
                    QMessageBox::critical(0, QObject::tr("Update All Customer Database"),updateid_query.lastError().text());

            }
        }
   }
}
