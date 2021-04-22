#include "commandec.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QTextDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QDateTime>
commandeC::commandeC()
{
    ID="";
    GATEAUXNAME="";
    SOMME=0;
    QUANTC=0;
}

commandeC::commandeC(QString ID, QString GATEAUXNAME, int SOMME, int QUANTC)
{
    this->ID = ID;
    this->GATEAUXNAME = GATEAUXNAME;
    this->SOMME = SOMME;
    this->QUANTC = QUANTC;
}

QString commandeC::get_id()
{
    return ID;
}
QString commandeC::get_namegat()
{
    return GATEAUXNAME;
}
int commandeC::get_somme()
{
    return SOMME;
}
int commandeC::get_quantc()
{
    return QUANTC;
}

bool commandeC::add_commande()
{
    QSqlQuery query;
    query.prepare("INSERT INTO COMMANDE (ID,GATEAUXNAME,SOMME,QUANTC)"
                  "VALUES(:ID,:GATEAUXNAME,:SOMME,:QUANTC)");
    query.bindValue(":ID",ID);
    query.bindValue(":GATEAUXNAME",GATEAUXNAME);
    query.bindValue(":SOMME",SOMME);
    query.bindValue(":QUANTC",QUANTC);
    return    query.exec();
}
QSqlQueryModel * commandeC::showc()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMMANDE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("GATEUXNAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("SOMME"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("QUANTC"));
        return model;
}
bool commandeC::removec(int id)
{
QSqlQuery query;
QString toSearch= QString::number(id);
query.prepare("DELETE FROM COMMANDE WHERE ID = :ID");
query.bindValue(":ID", toSearch);
return    query.exec();
}

bool commandeC::modifyc(){
    QSqlQuery query;
    query.prepare("UPDATE COMMANDE SET ID= :ID, GATEAUXNAME= :GATEAUXNAME, SOMME= :SOMME, QUANTC= :QUANTC WHERE ID= :ID");
    query.bindValue(":ID",ID);
    query.bindValue(":GATEAUXNAME",GATEAUXNAME);
    query.bindValue(":SOMME",SOMME);
    query.bindValue(":QUANTC",QUANTC);
    return query.exec();
}
QSqlQueryModel * commandeC::search(const QString &fullname)
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from COMMANDE where (GATEAUXNAME LIKE '%"+fullname+"%') ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("GATEAUXNAME"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("SOMME"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("QUANTC"));
     return model;
}
QSqlQueryModel * commandeC::show_Asc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from COMMANDE ORDER BY SOMME ASC ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("GATEAUXNAME"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("SOMME"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("QUANTC"));
     return model;
}
bool commandeC::pdfunction(QString &name, QString &path)
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
   printer.setOutputFileName(path);
   QString currentDate = QDateTime().currentDateTime().toString();
    QPainter painter ;
    QPixmap logo(":/img/img/logo.png");
    painter.begin(&printer);

    QFont font;
    QSqlQuery query;
    QString  gateauxname_pdf, quantc_pdf, somme_pdf;

        if(name !=""){
        query.prepare("select * from COMMANDE where ID='"+name+"'");
        if (query.exec())
        {
            while (query.next())
            {
                gateauxname_pdf=query.value(1).toString();
                quantc_pdf=query.value(2).toString();
                somme_pdf=query.value(3).toString();

            }
        }
        font.setPixelSize(35);
        painter.setFont(font);
        painter.setPen(Qt::darkCyan);
        painter.drawText(200,100,currentDate);
        painter.drawText(300,150,"BILL");

        font.setPixelSize(22);
        painter.setFont(font);
        painter.setPen(Qt::darkGreen);
        painter.drawPixmap(10,10,100,100, logo);
        painter.drawText(150,300,"Cake Name:");
        painter.drawText(150,400,"Quantity :");
        painter.drawText(150,500,"Total:");


        font.setPixelSize(19);
        painter.setFont(font);
        painter.setPen(Qt::gray);
        painter.drawText(300,300,gateauxname_pdf);
        painter.drawText(290,400,somme_pdf);
        painter.drawText(250,500,quantc_pdf);

    painter.end();
        }
}
void commandeC::show_notification(QString titre,QString text)
{
    this->text=text;
      this->titre=titre;
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
    notifyIcon->setIcon(QIcon(":/img/img/logo.png"));
    notifyIcon->show();
    notifyIcon->showMessage(titre,text,QSystemTrayIcon::Information,15000);
}
