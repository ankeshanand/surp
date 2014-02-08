#ifndef CHECKOUT_H
#define CHECKOUT_H
#include <string>
#include <QtGui>
#include "databasemanager.h"

using namespace std;

class CheckOut:public QObject
{   Q_OBJECT
private:
    string CustomerToken;
    string CustomerName;
    float bill;
    float advance;
    float discount;
    QString rooms;
    QStringList cateringOrder;
    QLabel *discountamountLabel;
    QLabel *amountLabel;
    QWidget *CustomerBill;
    QPushButton *payButton;

private slots:
    void editDiscount(QString );
    void deleteCustomer();

public:
    CheckOut(string);
    bool findCustomer();
    void provideDiscount();
    void generateBill();
};

#endif // CHECKOUT_H
