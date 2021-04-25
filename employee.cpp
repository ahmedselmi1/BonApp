#include "employee.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QPixmap>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextStream>
#include <QDebug>
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QVector>
#include "QCustomPlot.h"

employee::employee()
{
    cin="";
    age=0;
    phone=0;
    fullname="";
    email="";
    address="";
}

employee::employee(QString cin, QString fullname, int age, QString email, int phone, QString address)
{
    this->cin=cin;
    this->fullname=fullname;
    this->age=age;
    this->email=email;
    this->phone=phone;
    this->address=address;
}

QString employee::get_cin(){return cin;}
QString employee::get_fullname(){return fullname;}
int employee::get_age(){return age;}
QString employee::get_email(){return email;}
int employee::get_phone(){return phone;}
QString employee::get_address(){return address;}

bool employee::add(){
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEE (CIN, FULLNAME, AGE, EMAIL, PHONE, ADDRESS)"
                  "VALUES(:CIN, :FULLNAME, :AGE, :EMAIL, :PHONE, :ADDRESS)");
    query.bindValue(":CIN",cin);
    query.bindValue(":FULLNAME",fullname);
    query.bindValue(":AGE",age);
    query.bindValue(":EMAIL",email);
    query.bindValue(":PHONE",phone);
    query.bindValue(":ADDRESS",address);
    return query.exec();
}

bool employee::remove(QString toSearch)
{
QSqlQuery query;
query.prepare("DELETE FROM EMPLOYEE WHERE CIN = :CIN");
query.bindValue(":CIN", toSearch);
return    query.exec();
}

bool employee::modify(){
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET CIN= :CIN, FULLNAME= :FULLNAME, AGE= :AGE, EMAIL= :EMAIL, PHONE= :PHONE, ADDRESS= :ADDRESS WHERE CIN= :CIN");
    query.bindValue(":CIN",cin);
    query.bindValue(":FULLNAME",fullname);
    query.bindValue(":AGE",age);
    query.bindValue(":EMAIL",email);
    query.bindValue(":PHONE",phone);
    query.bindValue(":ADDRESS",address);
    return query.exec();
}

QSqlQueryModel * employee::search(const QString &fullname)
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from employee where (FULLNAME LIKE '"+fullname+"%') ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
     model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone Number"));
     model->setHeaderData(5, Qt::Horizontal, QObject::tr("Address"));    
     return model;
}

QSqlQueryModel * employee::show_Asc()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYEE ORDER BY AGE ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone Number"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Address"));
        return model;
}

QSqlQueryModel * employee::show_Desc()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYEE ORDER BY AGE DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone Number"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Address"));
        return model;
}

QSqlQueryModel * employee::show()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYEE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone Number"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Address"));
        return model;
}

QSqlQueryModel * employee::show_shifts()
{
    QSqlQueryModel * mod= new QSqlQueryModel();
    mod->setQuery("SELECT EMPLOYEE.CIN, EMPLOYEE.FULLNAME, SHIFTS.STARTDAY FROM EMPLOYEE FULL OUTER JOIN SHIFTS ON EMPLOYEE.CIN=SHIFTS.CIN");
    mod->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    mod->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    mod->setHeaderData(2, Qt::Horizontal, QObject::tr("Start Day"));
        return mod;
}

void employee::show_notification(QString titre,QString text)
{
    this->text=text;
      this->titre=titre;
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
    notifyIcon->setIcon(QIcon(":/img/img/logo.png"));
    notifyIcon->show();
    notifyIcon->showMessage(titre,text,QSystemTrayIcon::Information,15000);
}

QSqlQuery employee::getStat_1(int &stat1)
{
    stat1 = 0;
    QSqlQuery query1("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='20'");
    while(query1.next())
    {
        stat1 = query1.value(0).toInt();
    }
    return query1;
}

QSqlQuery employee::getStat_2(int &stat2)
{
    stat2 = 0;
    QSqlQuery query2("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='21'");
    while(query2.next())
    {
        stat2 = query2.value(0).toInt();
    }
    return query2;
}

QSqlQuery employee::getStat_3(int &stat3)
{
    stat3 = 0;
    QSqlQuery query3("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='22'");
    while(query3.next())
    {
        stat3 = query3.value(0).toInt();
    }
    return query3;
}

QSqlQuery employee::getStat_4(int &stat4)
{
    stat4 = 0;
    QSqlQuery query4("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='23'");
    while(query4.next())
    {
        stat4 = query4.value(0).toInt();
    }
    return query4;
}

QSqlQuery employee::getStat_5(int &stat5)
{
    stat5 = 0;
    QSqlQuery query5("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='24'");
    while(query5.next())
    {
        stat5 = query5.value(0).toInt();
    }
    return query5;
}

QSqlQuery employee::getStat_6(int &stat6)
{
    stat6 = 0;
    QSqlQuery query6("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='25'");
    while(query6.next())
    {
        stat6 = query6.value(0).toInt();
    }
    return query6;
}

QSqlQuery employee::getStat_7(int &stat7)
{
    stat7 = 0;
    QSqlQuery query7("SELECT COUNT(*) FROM EMPLOYEE WHERE AGE='26'");
    while(query7.next())
    {
        stat7 = query7.value(0).toInt();
    }
    return query7;
}

