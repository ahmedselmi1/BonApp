#include "category.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
category::category()
{
    id="";
    type="";
}

category::category(QString id, QString type)
{
    this->id = id;
    this->type = type;
}

QString category::get_id()
{
    return id;
}
QString category::get_type()
{
    return type;
}
bool category::add_category()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CATEGORY (id,type)"
                  "VALUES(:id,:type)");
    query.bindValue(":id",id);
    query.bindValue(":type",type);
    return    query.exec();
}
QSqlQueryModel * category::show()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM CATEGORY");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("type"));
        return model;
}
bool category::remove(int id)
{
QSqlQuery query;
QString toSearch= QString::number(id);
query.prepare("DELETE FROM CATEGORY WHERE ID = :ID");
query.bindValue(":ID", toSearch);
return    query.exec();
}

bool category::modify(){
    QSqlQuery query;
    query.prepare("UPDATE CATEGORY SET ID= :ID, TYPE= :TYPE WHERE ID= :ID");
    query.bindValue(":ID",id);
    query.bindValue(":TYPE",type);
    return query.exec();
}
QSqlQueryModel * category::search(const QString &fullname)
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from CATEGORY where (TYPE LIKE '%"+fullname+"%') ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE"));
     return model;
}
QSqlQueryModel * category::show_Desc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from CATEGORY ORDER BY TYPE DESC ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE"));
     return model;
}
QSqlQuery category::stat_1(int &stat1)
{
    stat1=0;
    QSqlQuery query1("SELECT COUNT(*) FROM GATEAUX WHERE PRICE<20");
    while(query1.next())
    {
        stat1=query1.value(0).toInt();
    }
    return query1;
}

QSqlQuery category::stat_2(int &stat2)
{
    stat2=0;
    QSqlQuery query1("SELECT COUNT(*) FROM GATEAUX WHERE PRICE<20");
    while(query1.next())
    {
        stat2=query1.value(0).toInt();
    }
    return query1;
}
QSqlQuery category::stat_3(int &stat3)
{
    stat3=0;
    QSqlQuery query1("SELECT COUNT(*) FROM GATEAUX WHERE PRICE<20");
    while(query1.next())
    {
        stat3=query1.value(0).toInt();
    }
    return query1;
}
void category::show_notification(QString titre,QString text)
{
    this->text=text;
      this->titre=titre;
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
    notifyIcon->setIcon(QIcon(":/img/img/logo app.png"));
    notifyIcon->show();
    notifyIcon->showMessage(titre,text,QSystemTrayIcon::Information,15000);
}

