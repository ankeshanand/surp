#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "management.h"
#include "cateringservices.h"
#include "checkout.h"

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QTabBar>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QString>
#include <QMessageBox>
#include <QString>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QScrollArea>
#include <QDateEdit>

class CateringServices;

class MainWindow : public QMainWindow
{  Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
    void managerLoginPage();
    void receptionPage();
    void reservationPage();
    void checkinPage();
    void checkoutPage();
    void cateringPage();
    void managerAfterLoginPage();
    void occupancyPage();
    void thresholdPage();
    void updateVisit_MonthCount();

private slots:
    void addItem(const QString&);
    void addRoom(const QString&);
    void createOrder();
    void checkPassword();
    void checkout();
    void checkin();
    void checkAvailability();
    void setValue(const int);
    void reviseRates();
    void calculateBill();
    void showAverageOccupancy();
    void updateThresholdValue();
    void updateDatabase();

private:
    QTabWidget *managerLoginWidget;
    QWidget *receptionWidget;
    QWidget *cateringWidget;
    QWidget *checkinWidget;
    QWidget *checkoutWidget;
    QWidget *reservationWidget;
    QTabWidget *reception;

    //manager login page widgets
    QLabel *passwordLabel;
    QLineEdit *passwordField;
    QPushButton *passwordButton;
    QLineEdit *averageOccupancyField;
    QLineEdit *reviseRatesField;
    QComboBox *monthCombo;
    QWidget *occupancyWidget;
    QWidget *thresholdWidget;
    QButtonGroup *reviseRatesGroup;
    QSpinBox *thresholdSpin;

    //reception page widgets
    QLabel *nameLabel;
    QLabel *checkInDateLabel;
    QLabel *checkOutDateLabel;
    QLabel *numberOfRoomsLabel;
    QLabel *customerTokenLabel;
    QLabel *advanceLabel;
    QLabel **roomDescriptionLabel;
    QLineEdit *nameField;
    QDateEdit *checkInDateField;
    QDateEdit *checkOutDateField;
    QLineEdit *customerTokenField;
    QLineEdit *numberOfRoomsField;
    QLineEdit *checkoutTokenField;
    QLineEdit *advanceField;
    QComboBox **typeOfRoom;
    QComboBox **bedOfRoom;
    QWidget *room;
    QPushButton *checkAvailabilitybutton;
    QPushButton *checkinButton;
    QPushButton *checkoutButton;

    //catering page widgets

    QLabel *numberOfItemsLabel;
    QLineEdit *numberOfItemsField;
    QLabel **itemDescriptionLabel;
    QLineEdit **itemDescriptionField;
    QLabel **itemNumberLabel;
    QSpinBox **itemNumberField;
    QPushButton *createOrderButton;
    QWidget *item;
    QLabel *cateringPageTokenLabel;
    QLineEdit *cateringPageTokenField;
    QLabel **individualBillLabel;
    QLineEdit **individualBillField;
    QLineEdit *totalBillField;
    QLabel *totalBillLabel;

    Management *manager;
    CateringServices *cateringServices;


};


#endif // MAINWINDOW_H
