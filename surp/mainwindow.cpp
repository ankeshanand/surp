#include "mainwindow.h"
#include "management.h"
#include "reservation.h"
#include "room.h"
#include "checkin.h"
#include <QFrame>
using namespace std;

//constructor
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    managerLoginPage();
    receptionPage();
    cateringPage();

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->insertTab(0, receptionWidget,tr("&Reception"));
    tabWidget->insertTab(1, managerLoginWidget, tr("Manager Login"));
    tabWidget->insertTab(2, cateringWidget, tr("Catering Services"));
    tabWidget->setTabEnabled(0, true);


    setCentralWidget(tabWidget);
    cateringServices = new CateringServices;   //A universal catering service object. Reinitailise for new use.
    manager = new Management;

    updateVisit_MonthCount();          //update month count for token

}

void MainWindow::updateVisit_MonthCount()
{   int i;
    QSqlQuery query1("SELECT Last_Date FROM Management");
    if(query1.next())
        i = QDate::currentDate().month()-query1.value(0).toDate().month();


    QSqlQuery query2("UPDATE All_Customer SET Visit_MonthCount=Visit_MonthCount + "+QString::number(i));
}

void MainWindow::updateDatabase()
{
    QSqlQuery query1("UPDATE Management SET Last_Date = '"+QDate::currentDate().toString(Qt::ISODate)+"'");
}

//add room dynamically on making reservation
void MainWindow::addRoom(const QString & number)
{
        delete room;
        delete checkAvailabilitybutton;
        roomDescriptionLabel = new QLabel*[number.toInt()];
        typeOfRoom = new QComboBox*[number.toInt()];
        bedOfRoom = new QComboBox*[number.toInt()];
        QHBoxLayout **layout = new QHBoxLayout*[number.toInt()];
        QVBoxLayout *mainlayout = new QVBoxLayout;
        for(int i=0;i<number.toInt();i++){
            roomDescriptionLabel[i] = new QLabel(tr("Room Description : "));
            typeOfRoom[i] = new QComboBox;
            typeOfRoom[i]->addItem("Non-AC");
            typeOfRoom[i]->addItem("AC");
            bedOfRoom[i] = new QComboBox;
            bedOfRoom[i]->addItem("Single");
            bedOfRoom[i]->addItem("Double");
            layout[i] = new QHBoxLayout;
            layout[i]->addWidget(roomDescriptionLabel[i]);
            layout[i]->addWidget(typeOfRoom[i]);
            layout[i]->addWidget(bedOfRoom[i]);
            mainlayout->addLayout(layout[i]);
        }
        checkAvailabilitybutton = new QPushButton(tr("&Check Availability"));
        connect(checkAvailabilitybutton,SIGNAL(clicked()),this,SLOT(checkAvailability()));
        room = new QWidget;
        room->setLayout(mainlayout);
        reservationWidget->layout()->addWidget(room);
        if(number.toInt())
            reservationWidget->layout()->addWidget(checkAvailabilitybutton);
}



/*------------- Slots------------*/

//check out slot
void MainWindow::checkout(){
    new CheckOut(checkoutTokenField->text().toStdString());
}

//checkin slot
void MainWindow::checkin(){
    new CheckIn(customerTokenField->text().toStdString(),advanceField->text().toFloat());
}

//checkAvailability slot
void MainWindow::checkAvailability(){
    Room *rooms = new Room[numberOfRoomsField->text().toInt()];
    for(int i =0;i<numberOfRoomsField->text().toInt();i++)
        rooms[i].RoomType = (int)typeOfRoom[i]->currentIndex()*2+(int)bedOfRoom[i]->currentIndex()+1;
    new Reservation(nameField->text().toStdString(),checkInDateField->date().toString(Qt::ISODate).toStdString(),
                    checkOutDateField->date().toString(Qt::ISODate).toStdString(),numberOfRoomsField->text().toInt(),rooms);

}

//checkPassword of manager slot
void MainWindow::checkPassword()
{
   if((passwordField->text().compare("manager")))
           QMessageBox::critical(0, QObject::tr("Invalid Login"),"wrong password!!!");
   else
   {
       managerAfterLoginPage();
   }
}

//set value of occupacy as month is changed
void MainWindow::setValue(const int i)
{
    averageOccupancyField->setText(QString::number(manager->getAverageOccupancy(i)));
}




/*------------ Tabs and Pages---------------*/
//catering services
void MainWindow::cateringPage()
{
    cateringPageTokenLabel = new QLabel("Customer Token : ");
    cateringPageTokenField = new QLineEdit(tr(""));

    numberOfItemsLabel = new QLabel(tr("Number Of Items : "));
    numberOfItemsField = new QLineEdit;

    createOrderButton = new QPushButton(tr("Create Order"));

    connect(numberOfItemsField,SIGNAL(textChanged(const QString&)),this,SLOT(addItem(const QString&)));
    item = new QWidget;

    QHBoxLayout *horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->addWidget(cateringPageTokenLabel);
    horizontalLayout1->addWidget(cateringPageTokenField);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->addWidget(numberOfItemsLabel);
    horizontalLayout2->addWidget(numberOfItemsField);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(horizontalLayout1);
    verticalLayout->addLayout(horizontalLayout2);


    cateringWidget = new QWidget;
    cateringWidget->setLayout(verticalLayout);

}

void MainWindow::createOrder()
{

    cateringServices->reinitialiseData();
    QString tnumber = cateringPageTokenField->text();   //token number

    QList<QPair<QString, int> > orderDescription;
    int length = numberOfItemsField->text().toInt();
    for (int i=0; i<length ;i++)
    {
        QPair<QString, int> q(itemDescriptionField[i]->text(), itemNumberField[i]->text().toInt());
        orderDescription.push_back(q);
    }
    bool success = cateringServices->updateBill(tnumber, orderDescription, totalBillField->text().toInt());
    cateringPageTokenField->setText("");

    //if bill is not updated, another chance will be provided with the item description and number fields remaining intact
    if(!success)
        return ;
    else
        QMessageBox::information(0,QObject::tr("Success"), QObject::tr("Service shall be provided shortly"), QMessageBox::Ok);


    numberOfItemsField->setText("");
    delete totalBillField;
    delete totalBillLabel;



}

//additem dynamically in catering order
void MainWindow::addItem(const QString& number)
{
        delete item;
        delete createOrderButton;

        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
        line->setFrameShadow(QFrame::Sunken);

        int orderLength = number.toInt();

        itemDescriptionLabel = new QLabel*[orderLength];
        itemDescriptionField = new QLineEdit*[orderLength];
        itemNumberLabel = new QLabel*[orderLength];
        itemNumberField = new QSpinBox*[orderLength];

        individualBillLabel = new QLabel*[orderLength];
        individualBillField = new QLineEdit*[orderLength];

        totalBillField = new QLineEdit;
        totalBillLabel = new QLabel("Order Bill : ");

        QHBoxLayout **layout = new QHBoxLayout*[2*orderLength];
        QVBoxLayout *mainlayout = new QVBoxLayout;


        for(int i=0;i<orderLength;i++)
        {
            QHBoxLayout *blayout1 = new QHBoxLayout;

            itemDescriptionLabel[i] = new QLabel;
            itemDescriptionField[i] = new QLineEdit;
            individualBillLabel[i] = new QLabel(QObject::tr("Individual Price:"));
            individualBillField[i] = new QLineEdit;
            itemNumberLabel[i] = new QLabel;
            itemNumberField[i] = new QSpinBox;

            itemNumberField[i]->setRange(0,10);

            layout[i] = new QHBoxLayout;
            layout[i+1] = new QHBoxLayout;

            itemDescriptionLabel[i]->setText(tr("Item Description : "));
            itemNumberLabel[i]->setText(tr("Number Of Items : "));

            blayout1->addWidget(individualBillLabel[i]);
            blayout1->addWidget(individualBillField[i]);

            layout[i]->addWidget(itemDescriptionLabel[i]);
            layout[i]->addWidget(itemDescriptionField[i]);

            layout[i+1]->addWidget(itemNumberLabel[i]);
            layout[i+1]->addWidget(itemNumberField[i]);

            mainlayout->addWidget(line);
            mainlayout->addLayout(layout[i]);
            mainlayout->addLayout(blayout1);
            mainlayout->addLayout(layout[i+1]);

            connect(itemNumberField[i], SIGNAL(valueChanged(int)), this, SLOT(calculateBill()));
        }


        QHBoxLayout *blayout2 = new QHBoxLayout;
        blayout2->addWidget(totalBillLabel);
        blayout2->addWidget(totalBillField);
        mainlayout->addLayout(blayout2);
        totalBillField->setReadOnly(true);

        createOrderButton = new QPushButton(tr("Create Order"));

        connect(createOrderButton,SIGNAL(clicked()),this,SLOT(createOrder()));

        item = new QWidget;
        item->setLayout(mainlayout);
        cateringWidget->layout()->addWidget(item);

        if(number.toInt())
            cateringWidget->layout()->addWidget(createOrderButton);
}

void MainWindow::calculateBill()
{
    bool ok;
    int length = numberOfItemsField->text().toInt(&ok, 10);
    if(!ok)
        QMessageBox::information(0, "Input Error", "Please Enter number of items properly");
    else
    {
        int i, orderBill = 0;
        for(i=0; i<length; i++)
        {
            orderBill = orderBill + (individualBillField[i]->text().toInt(&ok, 10)*itemNumberField[i]->value());
        }
        totalBillField->setText(QString::number(orderBill));
    }
}

// reception Maintab
void MainWindow::receptionPage()
{
    reservationPage();
    checkinPage();
    checkoutPage();
    reception = new QTabWidget;
    reception->insertTab(0,reservationWidget,tr("Reservation"));
    reception->insertTab(1,checkinWidget,tr("Check In"));
    reception->insertTab(2,checkoutWidget,tr("Check Out"));
    reception->setTabEnabled(0,true);
    receptionWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(reception);
    receptionWidget->setLayout(layout);
}

//reception make reservation Tab

void MainWindow::reservationPage()
{
    nameLabel = new QLabel(tr(              "Name :                         "));
    nameField = new QLineEdit(tr(""));
    checkInDateLabel = new QLabel(tr(       "Check-in date :          "));
    checkInDateField = new QDateEdit;
    checkOutDateLabel = new QLabel(tr(      "Check-out date :       "));
    checkOutDateField = new QDateEdit;
    numberOfRoomsLabel = new QLabel(tr("Number Of Rooms : "));
    numberOfRoomsField = new QLineEdit;
    checkAvailabilitybutton = new QPushButton(tr("Check Availability"));
    checkOutDateField->setDate(QDate::currentDate());
    checkInDateField->setDate(QDate::currentDate());

    QHBoxLayout *lineWidget1 = new QHBoxLayout;
    lineWidget1->addWidget(nameLabel);
    lineWidget1->addWidget(nameField);

    QHBoxLayout *lineWidget2 = new QHBoxLayout;
    lineWidget2->addWidget(checkInDateLabel);
    lineWidget2->addWidget(checkInDateField);

    QHBoxLayout *lineWidget3 = new QHBoxLayout;
    lineWidget3->addWidget(checkOutDateLabel);
    lineWidget3->addWidget(checkOutDateField);

    QHBoxLayout *lineWidget4 = new QHBoxLayout;
    lineWidget4->addWidget(numberOfRoomsLabel);
    lineWidget4->addWidget(numberOfRoomsField);

    QVBoxLayout *verticalLayoutManager = new QVBoxLayout;
    verticalLayoutManager->addLayout(lineWidget1);
    verticalLayoutManager->addLayout(lineWidget2);
    verticalLayoutManager->addLayout(lineWidget3);
    verticalLayoutManager->addLayout(lineWidget4);

    room = new QWidget;
    connect(numberOfRoomsField,SIGNAL(textChanged(QString)),this,SLOT(addRoom(const QString &)));
    connect(checkAvailabilitybutton,SIGNAL(clicked()),this,SLOT(checkAvailability()));
    reservationWidget = new QWidget;
    reservationWidget->setLayout(verticalLayoutManager);
}


void MainWindow::checkinPage(){
    customerTokenLabel = new QLabel(tr("Customer Token :"));
    customerTokenField = new QLineEdit;
    advanceLabel = new QLabel(tr("Advance Amount"));
    advanceField = new QLineEdit(tr("0.00"));
    checkinButton = new QPushButton(tr("Check In"));
    QVBoxLayout *mainlayout = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(customerTokenLabel);
    layout->addWidget(customerTokenField);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(advanceLabel);
    layout1->addWidget(advanceField);

    mainlayout->addLayout(layout);
    mainlayout->addLayout(layout1);
    mainlayout->addWidget(checkinButton);
    connect(checkinButton,SIGNAL(clicked()),this,SLOT(checkin()));
    checkinWidget = new QWidget;
    checkinWidget->setLayout(mainlayout);
}

//reception checkout subtab

void MainWindow::checkoutPage(){
    customerTokenLabel = new QLabel(tr("Customer Token :"));
    checkoutTokenField = new QLineEdit;
    checkoutButton = new QPushButton(tr("Check Out"));
    QVBoxLayout *mainlayout = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(customerTokenLabel);
    layout->addWidget(checkoutTokenField);
    mainlayout->addLayout(layout);
    mainlayout->addWidget(checkoutButton);
    connect(checkoutButton,SIGNAL(clicked()),this,SLOT(checkout()));
    checkoutWidget = new QWidget;
    checkoutWidget->setLayout(mainlayout);
}

//manager login Page
void MainWindow::managerLoginPage(){
    passwordLabel = new QLabel(tr("Enter Password : "));
    passwordField = new QLineEdit(tr("password"));
    passwordField->setEchoMode(QLineEdit::Password);
    passwordButton = new QPushButton(tr("Enter"));
    QVBoxLayout *verticalLayoutManager = new QVBoxLayout;
    QHBoxLayout *horizontalLayoutManager = new QHBoxLayout;
    managerLoginWidget = new QTabWidget;
    connect(passwordButton,SIGNAL(clicked()),this,SLOT(checkPassword()));
    horizontalLayoutManager->addWidget(passwordLabel);
    horizontalLayoutManager->addWidget(passwordField);
    verticalLayoutManager->addLayout(horizontalLayoutManager);
    verticalLayoutManager->addWidget(passwordButton, Qt::AlignCenter);
    managerLoginWidget->setLayout(verticalLayoutManager);
}

//manager after login screen
void MainWindow::managerAfterLoginPage(){

    delete passwordLabel;
    delete passwordField;
    delete passwordButton;
    occupancyPage();
    thresholdPage();
    managerLoginWidget->insertTab(0,occupancyWidget,tr("Revise Tariff"));
    managerLoginWidget->insertTab(1,thresholdWidget,tr("Threshold"));
    managerLoginWidget->setTabEnabled(0,true);
}

//revise Tariff
void MainWindow::reviseRates()
{
    manager->updateTariff(reviseRatesField->text().toFloat(),reviseRatesGroup->checkedId());
    QMessageBox::information(0, QObject::tr("SUCCESS"), QObject::tr("Tariff Updated."), QMessageBox::Ok);
}


void MainWindow::showAverageOccupancy()
{
    averageOccupancyField->setText(QString::number(manager->getAverageOccupancy(monthCombo->currentIndex())));
}

//Revise Rates tab
void MainWindow::occupancyPage()
{
    QLabel *monthLabel = new QLabel(tr("Month : "));
    QLabel *averageOccupancyLabel = new QLabel(tr("Average Occupancy : "));
    monthCombo = new QComboBox;
    averageOccupancyField = new QLineEdit;
    QStringList months;
    months <<"January"<<"February"<<"March"<<"April"<<"May"<<"June"<<"July"<<"August"<<"September"<<"October"<<"November"<<"December";
    monthCombo->addItems(months);
    connect(monthCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setValue(const int)));
    connect(monthCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(showAverageOccupancy()));
    monthCombo->setCurrentIndex(0);
    averageOccupancyField->setText(QString::number(manager->getAverageOccupancy(0)));
    averageOccupancyField->setReadOnly(true);

    QLabel *reviseRatesLabel = new QLabel;
    reviseRatesLabel->setText(tr("Revise Rates By % : "));

    reviseRatesField = new QLineEdit(tr("0"));

    QRadioButton *reviseRatesUpRadio = new QRadioButton;
    reviseRatesUpRadio->setText(tr("Increment"));

    QRadioButton *reviseRatesDownRadio = new QRadioButton;
    reviseRatesDownRadio->setText(tr("Decrement"));

    reviseRatesGroup = new QButtonGroup;
    reviseRatesGroup->addButton(reviseRatesUpRadio,0);
    reviseRatesGroup->addButton(reviseRatesDownRadio,1);
    reviseRatesUpRadio->setChecked(true);

    QPushButton *reviseRatesButton = new QPushButton(tr("Revise Tariff"));
    connect(reviseRatesButton,SIGNAL(clicked()),this,SLOT(reviseRates()));
    connect(reviseRatesField, SIGNAL(returnPressed()), this, SLOT(reviseRates()));
    occupancyWidget = new QWidget;

    QHBoxLayout *horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->addWidget(monthLabel);
    horizontalLayout1->addWidget(monthCombo);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->addWidget(averageOccupancyLabel);
    horizontalLayout2->addWidget(averageOccupancyField);

    QHBoxLayout *horizontalLayout3 = new QHBoxLayout;
    horizontalLayout3->addWidget(reviseRatesLabel);
    horizontalLayout3->addWidget(reviseRatesField);
    horizontalLayout3->addWidget(reviseRatesUpRadio);
    horizontalLayout3->addWidget(reviseRatesDownRadio);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(horizontalLayout1);
    verticalLayout->addLayout(horizontalLayout2);
    verticalLayout->addLayout(horizontalLayout3);
    verticalLayout->addWidget(reviseRatesButton);

    occupancyWidget->setLayout(verticalLayout);

}

void MainWindow::thresholdPage()
{
    QLabel *thresholdLabel = new QLabel(QObject::tr("Enter Threshold Value : "));
    thresholdSpin = new QSpinBox;
    QPushButton *thresholdButton = new QPushButton(QObject::tr("Update Threshold"));
    thresholdSpin->setRange(1,30);

    QHBoxLayout *thresholdHLayout = new QHBoxLayout;
    thresholdHLayout->addWidget(thresholdLabel);
    thresholdHLayout->addWidget(thresholdSpin);

    QVBoxLayout *thresholdVLayout = new QVBoxLayout;
    thresholdVLayout->addLayout(thresholdHLayout);
    thresholdVLayout->addWidget(thresholdButton);

    thresholdWidget = new QWidget;
    thresholdWidget->setLayout(thresholdVLayout);

    connect(thresholdSpin,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));
    connect(thresholdButton, SIGNAL(clicked()), this, SLOT(updateThresholdValue()));
}

void MainWindow::updateThresholdValue()
{
    if(manager->updateThreshold(thresholdSpin->value()))
        QMessageBox::information(0,QObject::tr("Success"), QObject::tr("Threshold Value Updated."));
    else
        QMessageBox::critical(0,QObject::tr("Failure"), QObject::tr("Threshold Value Could not be updated."));
}
