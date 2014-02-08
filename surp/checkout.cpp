#include "checkout.h"

CheckOut::CheckOut(string token):QObject()
{   CustomerToken = token;
    // find the corresponding customer with this customer token
    if(!findCustomer())         // if customer is not found then display message of invalid token
        QMessageBox::critical(0,QObject::tr("Check Out"),"Invalid Customer Token");

    else{   //if found
            generateBill();     // generate bill of the customer
            payButton = new QPushButton("Pay");         // payment button
            //connecting paybutton to slot as paybutton is clicked corresponding customer is deleted
            QObject::connect(payButton,SIGNAL(clicked()),this,SLOT(deleteCustomer()));
            CustomerBill->layout()->addWidget(payButton);
        }
}

// function to find customer of the corresponding token
bool CheckOut::findCustomer()
{
    //connect to database
    DatabaseManager *dbm = DatabaseManager::Instance();
    //query to get customer information from residing customer
    QSqlQuery query("SELECT * FROM `Residing_Customer` WHERE Customer_Token = '"+QString::fromStdString(CustomerToken)+"'",dbm->db);
    if(!query.isActive())
        QMessageBox::critical(0,QObject::tr("Check Out"),query.lastError().text());
    if(!query.next())   //if query is empty return false
        return false;
    //else store the customer info into data memebers and return true as customer found
    CustomerName = query.value(query.record().indexOf("Name")).toString().toStdString();
    advance = query.value(query.record().indexOf("Advance")).toFloat();
    bill = query.value(query.record().indexOf("Bill")).toFloat();
    rooms = query.value(query.record().indexOf("Rooms")).toString();
    //spliting catering orders and insert in list
    cateringOrder = query.value(query.record().indexOf("Catering_Bill")).toString().split("OBJECT:",QString::SkipEmptyParts);
    return true;        //customer found

}

// function to generate bill as GUI
void CheckOut::generateBill()
{

        QLabel *Name = new QLabel(QObject::tr("Name : ")+QString::fromStdString(CustomerName));
        QLabel *Rooms = new QLabel(QObject::tr("Room Numbers : ")+rooms);
        QTableWidget *Bill = new QTableWidget;
        QStringList header;
        header<<"Order No"<<"Item Name"<<"Number Of Items"<<"Amount"<<"Date of Order";   // adding coulmn name in header list
        Bill->setColumnCount(5);
        Bill->setShowGrid(false);
        Bill->setHorizontalHeaderLabels(header);
        Bill->setRowCount(cateringOrder.size());

        //inserting items in Table
        for(int i = 0;i<cateringOrder.size();i++){
            QStringList cateringItem = cateringOrder.at(i).split(";");
            Bill->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
            for(int j = 0;j<cateringItem.size();j++){
                Bill->setItem(i,j+1,new QTableWidgetItem(cateringItem.at(j)));
            }

        }
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(Name);
        layout->addWidget(Rooms);
        layout->addWidget(Bill);

        CustomerBill = new QWidget;
        CustomerBill->setLayout(layout);
        provideDiscount();      //checking if customer is alloted customer id and if alloted provide discount
        CustomerBill->show();   //showing Bill

}

void CheckOut::provideDiscount(){
    DatabaseManager *dbm = DatabaseManager::Instance();

    discount = 0;
    //query to check Customer id exist for customer
    QSqlQuery query("SELECT * FROM `All_Customer` WHERE Name = '"+QString::fromStdString(CustomerName)+"'",dbm->db);

    if(!query.isActive())
        qDebug() << query.lastError().text();
    query.exec();
    // if customer id is not null then allow to provide discount
    if(!query.isNull(query.record().indexOf("Customer_Id"))){
        QLabel *discountLabel = new QLabel(QObject::tr("Discount % : "));
        QLineEdit *discountField = new QLineEdit(QObject::tr("0.00"));
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(discountLabel);
        layout->addWidget(discountField);
        QWidget *discountWidget = new QWidget;
        discountWidget->setLayout(layout);
        CustomerBill->layout()->addWidget(discountWidget);
        //connecting discount field to slot as discount as percentage is changed discount as amount is also changed
        QObject::connect(discountField,SIGNAL(textEdited(QString)),this,SLOT(editDiscount(QString)));
    }

    discountamountLabel = new QLabel;
    amountLabel = new QLabel;
    discountamountLabel = new QLabel(QObject::tr("Discount : ")+"0.00");
    amountLabel = new QLabel(QObject::tr("Amount To Pay : ")+QString::number(bill-advance));
    CustomerBill->layout()->addWidget(new QLabel(QObject::tr("Total Bill : ")+QString::number(bill)));
    CustomerBill->layout()->addWidget(new QLabel(QObject::tr("Advance Paid : ")+QString::number(advance)));
    CustomerBill->layout()->addWidget(discountamountLabel);
    CustomerBill->layout()->addWidget(amountLabel);

}

// slot to change discount amount as discount percentage is changed
void CheckOut::editDiscount(QString dis){
    discount = dis.toFloat()/100*(bill-advance);
    discountamountLabel->setText(QObject::tr("Discount : ")+QString::number(discount));
    amountLabel->setText(QObject::tr("Amount To Pay : ")+QString::number(bill-advance-discount));
}

//slot to delete customer from residing customer table and close bill
void CheckOut::deleteCustomer(){
        DatabaseManager *dbm = DatabaseManager::Instance();

        QSqlQuery query("DELETE FROM `Residing_Customer` WHERE Customer_Token = '"+QString::fromStdString(CustomerToken)+"'",dbm->db);
        query.exec();
        CustomerBill->close();
}
