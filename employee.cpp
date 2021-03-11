#include "employee.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

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
    mod->setQuery("SELECT CIN, FULLNAME FROM EMPLOYEE");
    mod->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    mod->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        return mod;
}

bool employee::remove(QString cin)
{
QSqlQuery query;
QString toSearch= cin;
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

