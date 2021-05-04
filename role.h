#ifndef ROLE_H
#define ROLE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QFrame>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>

using namespace std;

class role
{
public:
    role();
    role(QString,QString,QString,int,int);

    static int currentpage;
    static int maxPerPage;

    QString get_cin();
    QString get_name();
    QString get_role();
    int get_earning();
    int get_bonus();

    //crud functions
    bool add();
    bool modify();
    void remove(QString toSearch, QTableWidget *roleTable);
    void nextPage(QTableWidget* roleTable);
    void prevPage(QTableWidget* roleTable);
    void showPage(QTableWidget* roleTable);
    void getTable(QTableWidget* roleTable);
    void sortEarning(QTableWidget* roleTable);
    void sortRole(QTableWidget* roleTable);
    void search(QString toSearch,QTableWidget* roleTable);
    void stats(QFrame* roles);
    QSqlQuery getStat_1(int &);
    QSqlQuery getStat_2(int &);
    QSqlQuery getStat_3(int &);
private:
    QString cin,name,roles;
    int earning,bonus;
};

#endif // ROLE_H
