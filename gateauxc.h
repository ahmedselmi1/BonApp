#ifndef GATEAUXC_H
#define GATEAUXC_H
#include <QString>
#include <QSqlQuery>
#include <QSystemTrayIcon>
#include <QSqlQueryModel>

class gateauxC
{
public:
    gateauxC();
    gateauxC(QString,QString,int,int);
    QString get_id();
    QString get_name();
    int get_quantity();
    int get_price();
    bool add_gateaux();
    QSqlQuery stat_1(int &);
    QSqlQuery stat_2(int &);
    QSqlQuery stat_3(int &);
    //func
    QSqlQueryModel * show();
    bool remove(int);
    bool modify();
    QSqlQueryModel * search(const QString&);
    QSqlQueryModel * show_Desc();
    void show_notification(QString, QString);

private:
    QString ID,NAME,text,titre;
    int QUANTITY,PRICE;

};

#endif // GATEAUXC_H
