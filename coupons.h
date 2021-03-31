#ifndef COUPONS_H
#define COUPONS_H
#include <QTableWidget>


class coupons
{
public:
    coupons();
    static void deleteSelectedCoupons(QTableWidget*);
    static void processCouponTable(QTableWidget*);
    static bool addCouponToDB(QString code, QString totalNum, QString startDate, QString endDate, QString constraints);
    static void printPDF(QTableWidget* couponsTable);

};

#endif // COUPONS_H
