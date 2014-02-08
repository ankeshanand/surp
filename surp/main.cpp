#include <QApplication>
#include "mainwindow.h"
#include <QtSql>
#include <QtGui>
#include "databasemanager.h"


bool createConnection()
{
    DatabaseManager *dbm = DatabaseManager::Instance();
    if (!dbm->db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database create connection Error"),dbm->db.lastError().text());
        return false;
    }

    int i;

    QSqlQuery create_query("CREATE TABLE `Rooms` ( room_no int )",dbm->db);
    if (create_query.isActive())
    {
        for(i = 0;i<100;i++)
        {
            QSqlQuery query1("INSERT INTO `Rooms` (room_no) VALUES("+QString::number((i/25+1)*100+i%25)+")",dbm->db);
        }
    }

    QSqlQuery query("CREATE TABLE `Residing_Customer` ( Name varchar(40),Customer_Token varchar(20),CheckIn_Date text ,Checkout_Date text ,Number_Of_Rooms int ,Rooms varchar(255),Catering_Bill text,Advance REAL,Bill real)",dbm->db);
    if (query.isActive())
    {

    }


    QSqlQuery query1("CREATE TABLE `Management` ( Rates real, Average_Occupancy real,Current_Occupancy real,Threshold_Visit real, Last_Date varchar(40))",dbm->db);
    if (query1.isActive())
    {
        for(i = 0; i<12; i++)
        {
            if(i==0)
                QSqlQuery query2("INSERT INTO `Management` (Rates, Average_Occupancy, Current_Occupancy, Threshold_Visit)  VALUES(100, 0, 0, 3)");
            else if(i>=1 && i<=3)
                QSqlQuery query2("INSERT INTO `Management` (Rates, Average_Occupancy, Current_Occupancy) VALUES("+QString::number((i+1)*100)+",0,0)");
            else
                QSqlQuery query2("INSERT INTO `Management` (Average_Occupancy, Current_Occupancy) VALUES(0,0)");

        }
    }

    QSqlQuery query2("CREATE TABLE `All_Customer` ( Name varchar(40), Visits int, Visit_MonthCount int, Customer_Id varchar(20))");
    if (query2.isActive())
    {

    }

    return true;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(!createConnection())
        return 1;
    //QObject::connect(qApp, SIGNAL(aboutToQuit()), w, SLOT(updateDatabase()));
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(updateDatabase()));
    a.setApplicationName(QObject::tr("Mesonero"));
    w.resize(QSize(500, 500));
    w.show();


    return a.exec();
}
