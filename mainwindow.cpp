#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "shifts.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include<string>
#include<QDate>
#include <algorithm>
#include <vector>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//INDEXING

void MainWindow::on_back_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_goToEmployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_toolButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_back_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_toolButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_back_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//START EMPLOYEE

//FUNCTIONS

//show all employees

void MainWindow::on_toolButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->empTable->setModel(tableEmployee.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select CIN from EMPLOYEE"));
    ui->comboBox->setModel(mod);
}

//add employee + validation

void MainWindow::on_toolButton_save_2_clicked()
{
    QString cin= ui->lineEdit_cin->text();
    QString fullname= ui->lineEdit_fullname->text();
    int age= ui->lineEdit_age->text().toInt();
    QString email= ui->lineEdit_email->text();
    int phone= ui->lineEdit_phone->text().toInt();
    QString address= ui->lineEdit_address->text();

    int x=0;

    string str_phone = to_string(phone);
    string m = email.toStdString();

    if (cin.length()!=8)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("CIN must contain 8 numbers"), QMessageBox::Ok);
              x++;
    }

    else if (fullname=="")
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Name can't be empty"), QMessageBox::Ok);
              x++;
    }

    else if (age<18 || age>35)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Age must be between 18 and 35"), QMessageBox::Ok);
              x++;
    }

    else if (m.find("@gmail.com")== std:: string::npos)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Email must end with @gmail.com"), QMessageBox::Ok);
              x++;
    }

    else if (str_phone.length()!=8)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Phone number must be composed of 8 numbers"), QMessageBox::Ok);
              x++;
    }

    else if (address=="")
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Address can't be empty"), QMessageBox::Ok);
              x++;
    }

    else if(x==0)
    {
    employee e(cin,fullname,age,email,phone,address);
    bool toTest =e.add();
    if(toTest)
    {
        QMessageBox::information(nullptr, QObject::tr("Adding Employee"),
                          QObject::tr("Employee added successfully"), QMessageBox::Ok);
    }
    ui->lineEdit_cin->clear();
    ui->lineEdit_fullname->clear();
    ui->lineEdit_age->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_address->clear();
    ui->stackedWidget->setCurrentIndex(1);
    }
}

//delete employee (to change)

void MainWindow::on_toolButton_10_clicked()
{
    QString cin = ui->comboBox->currentText();
    tableEmployee.remove(cin);
    ui->empTable->setModel(tableEmployee.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select CIN from EMPLOYEE"));
    ui->comboBox->setModel(mod);
}

//modifying employee

void MainWindow::on_empTable_activated(const QModelIndex &index)
{
    QString val=ui->empTable->model()->data(index).toString();
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE CIN = '"+val+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->stackedWidget->setCurrentIndex(4);
            ui->lineEdit_id_m_2->setText(query.value(0).toString());
            ui->lineEdit_fullname_m->setText(query.value(1).toString());
            ui->lineEdit_age_m->setText(query.value(2).toString());
            ui->lineEdit_email_m->setText(query.value(3).toString());
            ui->lineEdit_phone_m->setText(query.value(4).toString());
            ui->lineEdit_address_m->setText(query.value(5).toString());
        }
    }
}

void MainWindow::on_toolButton_modify_clicked()
{
    QString cin = ui->lineEdit_id_m_2->text();
    QString fullname= ui->lineEdit_fullname_m->text();
    int age= ui->lineEdit_age_m->text().toInt();
    QString email= ui->lineEdit_email_m->text();
    int phone= ui->lineEdit_phone_m->text().toInt();
    QString address= ui->lineEdit_address_m->text();

    int x=0;

    string str_phone = to_string(phone);
    string m = email.toStdString();

    if (fullname=="")
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Name can't be empty"), QMessageBox::Ok);
              x++;
    }

    else if (age<18 || age>35)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Age must be between 18 and 35"), QMessageBox::Ok);
              x++;
    }

    else if (m.find("@gmail.com")== std:: string::npos)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Email must end with @gmail.com"), QMessageBox::Ok);
              x++;
    }

    else if (str_phone.length()!=8)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Phone number must be composed of 8 numbers"), QMessageBox::Ok);
              x++;
    }

    else if (address=="")
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Address can't be empty"), QMessageBox::Ok);
              x++;
    }

    else if(x==0)
    {
    employee e(cin,fullname,age,email,phone,address);
    bool toTest =e.modify();
    if(toTest)
    {
        QMessageBox::information(nullptr, QObject::tr("Editing Employee"),
                          QObject::tr("Employee's info edited successfully"), QMessageBox::Ok);
    }
    ui->lineEdit_fullname_m->clear();
    ui->lineEdit_age_m->clear();
    ui->lineEdit_email_m->clear();
    ui->lineEdit_phone_m->clear();
    ui->lineEdit_address_m->clear();
    ui->stackedWidget->setCurrentIndex(3);
    }
}

//ADVANCED FUNCTIONS

//search

void MainWindow::on_toolButton_11_clicked()
{
    QString fullname=ui->lineEdit_search->text();
    ui->empTable->setModel(tableEmployee.search(fullname));
}

//sort Desc

void MainWindow::on_toolButton_12_clicked()
{
    ui->empTable->setModel(tableEmployee.show_Desc());
}

//sort Asc

void MainWindow::on_toolButton_13_clicked()
{
    ui->empTable->setModel(tableEmployee.show_Asc());
}

//END EMPLOYEE

//START SHIFTS

//FUNCTIONS

//show employee id + add shifts

void MainWindow::on_toolButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    ui->emptable_2->setModel(tableEmployee1.show_shifts());
}

void MainWindow::on_toolButton_save_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    QString cin= ui->lineEdit_cin_shifts->text();
    QString jours= ui->calendarWidget_shifts->selectedDate().toString();
    QString hdeb=ui->timeEdit_hdeb->time().toString();
    QString hfin=ui->timeEdit_hfin->time().toString();

    shifts s(cin,jours,hdeb,hfin);
    s.add();
}

void MainWindow::on_emptable_2_activated(const QModelIndex &index)
{
    QString val=ui->emptable_2->model()->data(index).toString();
    QSqlQuery query;
    query.prepare("SELECT CIN FROM EMPLOYEE WHERE CIN = '"+val+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->lineEdit_cin_shifts->setText(query.value(0).toString());
        }
    }
}

//show employee + shifts

