#ifndef COUPONS_H
#define COUPONS_H
#include <QTableWidget>


class coupons
{
public:
    coupons();
    static int currentsorted;
    static int currentsorting;
    static int currentpage;
    static int maxPerPage;

    static void deleteSelectedCoupons(QTableWidget*);
    static void processCouponTable(QTableWidget*);
    static bool addCouponToDB(QString code, QString totalNum, QString startDate, QString endDate, QString constraints);
    static void printPDF(QTableWidget* couponsTable);
    static bool editCouponData(QTableWidget* couponsTable, int row);
    static void sortAccording(QTableWidget* couponsTable,int logicalIndex);
    static void nextPage(QTableWidget* couponsTable);
    static void prevPage(QTableWidget* couponsTable);
    static void showPage(QTableWidget* couponsTable);
    static void searchText(QTableWidget* couponsTable, QString textsearched);

};

#endif // COUPONS_H
