#include "cateringservices.h"
#include <QMessageBox>
#include <QtDebug>
#include <QtSql>

CateringServices::CateringServices()
{
    CustomerToken="";
    dbm = DatabaseManager::Instance();
    if(!dbm->db.open())
    {
        QMessageBox::critical(0,QObject::tr("Database Error"), dbm->db.lastError().text());
    }

    //create this database and table
    /*QString query("CREATE TABLE Residing_Customer ( Name varchar(40),Customer_Token varchar(40),
CheckIn_Date Date ,Checkout_Date Date , Number_Of_Rooms int,Rooms varchar(255),Catering_Bill text,Advance money, Bill text)");
    QSqlQuery *sql_query = new QSqlQuery(query, db);
    sql_query->exec();*/
}


void CateringServices::reinitialiseData()
{
    CustomerToken="";
}

bool CateringServices::updateBill(const QString& tokenNumber, const QList<QPair<QString, int> >& orderList, int orderBill)
{
    QPair<QString, int> pair;
    QString description="";
    QSqlQuery query1("SELECT Catering_Bill FROM Residing_Customer WHERE Customer_Token='"+tokenNumber+"'", dbm->db);

    /*extract the previous catering decription*/
    if(query1.next())
    {
        description = description + query1.value(0).toString();
        if(description==NULL)
            description="";
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Token Number Error"),QObject::tr("Please Enter Correct Token Number") );
        return false;
    }


    Q_FOREACH(pair, orderList)
    {
        description = description + "OBJECT:"+pair.first+";" + QString::number(pair.second) + ";" + QString::number(orderBill) + ";" + QDate::currentDate().toString(Qt::ISODate);
    }

    //update Catering Description
    QSqlQuery query2("UPDATE Residing_Customer SET Catering_Bill='"+description+"' WHERE Customer_Token='"+tokenNumber+"'", dbm->db);
    QSqlQuery query3("UPDATE Residing_Customer SET Bill = Bill + "+QString::number(orderBill)+" WHERE Customer_Token='"+tokenNumber+"'", dbm->db);

    return true;     //token number was correct and the bill description was successfully updated.
}
