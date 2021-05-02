#ifndef COUPONS_H
#define COUPONS_H
#include <QTableWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>



#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableView>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QVXYModelMapper>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QVBarModelMapper>
#include <QtWidgets/QHeaderView>
#include <QtCharts/QBarCategoryAxis>
using namespace QtCharts;





class coupons
{
public:
    coupons();
    static int currentsorted;
    static int currentsorting;
    static int currentpage;
    static int maxPerPage;

    static void deleteSelectedCoupons(QTableWidget*, QLayout* chartLayout);
    static void processCouponTable(QTableWidget*, QLayout* chartLayout);
    static bool addCouponToDB(QString code, QString totalNum, QString startDate, QString endDate, QString constraints,bool email);
    static void printPDF(QTableWidget* couponsTable);
    static bool editCouponData(QTableWidget* couponsTable, int row);
    static void sortAccording(QTableWidget* couponsTable,int logicalIndex);
    static void nextPage(QTableWidget* couponsTable);
    static void prevPage(QTableWidget* couponsTable);
    static void showPage(QTableWidget* couponsTable);
    static void searchText(QTableWidget* couponsTable, QString textsearched);


private:
    static QNetworkAccessManager *manager;
    static QNetworkRequest request;
    static QChartView *chartView;
};

#endif // COUPONS_H
