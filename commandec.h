#ifndef COMMANDEC_H
#define COMMANDEC_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSystemTrayIcon>

class commandeC
{
public:
    commandeC();
    commandeC(QString,QString,int,int);
    QString get_id();
    QString get_namegat();
    int get_somme();
    int get_quantc();
    bool add_commande();
    //func
    QSqlQueryModel * showc();
    bool removec(int);
    bool modifyc();
    QSqlQueryModel * search(const QString&);
    QSqlQueryModel * show_Asc();
    bool pdfunction(QString &,QString &);
    void show_notification(QString, QString);

private:
    QString ID,GATEAUXNAME,text,titre;
    int SOMME,QUANTC;

};

#endif // COMMANDEC_H
