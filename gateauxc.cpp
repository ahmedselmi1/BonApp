#include "gateauxc.h"
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
gateauxC::gateauxC()
{
    ID="";
    QUANTITY=0;
    NAME="";
    PRICE=0;
}

gateauxC::gateauxC(QString ID, QString NAME, int QUANTITY, int PRICE)
{
    this->ID = ID;
    this->NAME = NAME;
    this->QUANTITY = QUANTITY;
    this->PRICE = PRICE;
}

QString gateauxC::get_id()
{
    return ID;
}
QString gateauxC::get_name()
{
    return NAME;
}
int gateauxC::get_quantity()
{
    return QUANTITY;
}
int gateauxC::get_price()
{
    return PRICE;
}

bool gateauxC::add_gateaux()
{
    QSqlQuery query;
    query.prepare("INSERT INTO GATEAUX (ID,NAME,QUANTITY,PRICE)"
                  "VALUES(:ID,:NAME,:QUANTITY,:PRICE)");
    query.bindValue(":ID",ID);
    query.bindValue(":NAME",NAME);
    query.bindValue(":QUANTITY",QUANTITY);
    query.bindValue(":PRICE",PRICE);

    return    query.exec();
}
QSqlQueryModel * gateauxC::show()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM GATEAUX");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Quantity"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Price"));
        return model;
}
bool gateauxC::remove(int id)
{
QSqlQuery query;
QString toSearch= QString::number(id);
query.prepare("DELETE FROM GATEAUX WHERE ID = :ID");
query.bindValue(":ID", toSearch);
return    query.exec();
}

bool gateauxC::modify(){
    QSqlQuery query;
    query.prepare("UPDATE GATEAUX SET ID= :ID, NAME= :NAME, QUANTITY= :QUANTITY, PRICE= :PRICE WHERE ID= :ID");
    query.bindValue(":ID",ID);
    query.bindValue(":NAME",NAME);
    query.bindValue(":QUANTITY",QUANTITY);
    query.bindValue(":PRICE",PRICE);
    return query.exec();
}
QSqlQueryModel * gateauxC::search(const QString &fullname)
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from GATEAUX where (NAME LIKE '%"+fullname+"%') ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("NAME"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("QUANTITY"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRICE"));
     return model;
}
QSqlQueryModel * gateauxC::show_Desc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from GATEAUX ORDER BY QUANTITY DESC ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("NAME"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("QUANTITY"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRICE"));
     return model;
}
QSqlQuery gateauxC::stat_1(int &stat1)
{
    stat1=0;
    QSqlQuery query1("SELECT COUNT(*) FROM GATEAUX WHERE PRICE<20");
    while(query1.next())
    {
        stat1=query1.value(0).toInt();
    }
    return query1;
}

QSqlQuery gateauxC::stat_2(int &stat2)
{
    stat2=0;
    QSqlQuery query1("SELECT COUNT(*) FROM GATEAUX WHERE PRICE<20");
    while(query1.next())
    {
        stat2=query1.value(0).toInt();
    }
    return query1;
}
QSqlQuery gateauxC::stat_3(int &stat3)
{
    stat3=0;
    QSqlQuery query1("SELECT COUNT(*) FROM GATEAUX WHERE PRICE<20");
    while(query1.next())
    {
        stat3=query1.value(0).toInt();
    }
    return query1;
}

