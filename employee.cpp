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
#include <QMessageBox>
#include <QSystemTrayIcon>

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

bool employee::pdfunction(QString &name, QString &path)
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
   printer.setOutputFileName(path);
    QPainter painter ;
    QPixmap logo(":/img/img/logo.png");
    painter.begin(&printer);

    QFont font;
    QSqlQuery query;
    QString cin_pdf, fullname_pdf, age_pdf, email_pdf, phone_pdf, address_pdf ;

        if(name !=""){
        query.prepare("select * from employee where CIN='"+name+"'");
        if (query.exec())
        {
            while (query.next())
            {
                cin_pdf=query.value(0).toString();
                fullname_pdf=query.value(1).toString();
                age_pdf=query.value(2).toString();
                email_pdf=query.value(3).toString();
                phone_pdf=query.value(4).toString();
                address_pdf=query.value(5).toString();

            }
        }
        font.setPixelSize(50);
        painter.setFont(font);
        painter.setPen(Qt::red);
        painter.drawText(350,150,"Employee");

        font.setPixelSize(35);
        painter.setFont(font);
        painter.setPen(Qt::blue);
        painter.drawPixmap(10,10,50,50, logo);
        painter.drawText(20,300,"CIN :");
        painter.drawText(20,500,"Name :");
        painter.drawText(20,650,"Age :");
        painter.drawText(20,800,"Email :");
        painter.drawText(20,1050,"phone :");
        painter.drawText(20,1300,"Address :");


        font.setPixelSize(22);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(300,500,fullname_pdf);
        painter.drawText(300,300,cin_pdf);
        painter.drawText(300,650,age_pdf);
        painter.drawText(300,800,email_pdf);
        painter.drawText(300,1050,phone_pdf);
        painter.drawText(300,1300,address_pdf);

    painter.end();
        }

}

//notification

void employee::show_notification(QString titre,QString text)
{
    this->text=text;
      this->titre=titre;
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
    notifyIcon->setIcon(QIcon(":/img/img/logo.png"));
    notifyIcon->show();
    notifyIcon->showMessage(titre,text,QSystemTrayIcon::Information,15000);
}

