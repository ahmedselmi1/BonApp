#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "shifts.h"
#include <QTime>
#include <QTimeEdit>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include<string>
#include<QDate>
#include <algorithm>
#include <vector>
#include"QSqlRecord"
#include"QSqlQuery"
#include <QPixmap>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>

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


void MainWindow::on_back_18_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_back_16_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_back_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

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
        employee().show_notification("Add employee","Employee added successfully");
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

void MainWindow::on_empTable_clicked(const QModelIndex &index)
{
    QString val=ui->empTable->model()->data(index).toString();
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE CIN = '"+val+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->lineEdit->setText(query.value(0).toString());
        }
    }
}

void MainWindow::on_toolButton_10_clicked()
{
    QString cin = ui->lineEdit->text();
    if(cin=="")
    {
        employee().show_notification("Delete","no employee selected");
    }
    else
    {
        tableEmployee.remove(cin);
        employee().show_notification("Delete","employee deleted");
        ui->empTable->setModel(tableEmployee.show());
        ui->lineEdit->clear();
    }
}

//modifying employee + validation

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
        employee().show_notification("Modify employee","employee modified successfully");
    }
    ui->lineEdit_fullname_m->clear();
    ui->lineEdit_age_m->clear();
    ui->lineEdit_email_m->clear();
    ui->lineEdit_phone_m->clear();
    ui->lineEdit_address_m->clear();
    ui->stackedWidget->setCurrentIndex(3);
    ui->empTable->setModel(tableEmployee.show());
    }
}

//ADVANCED FUNCTIONS

//search

void MainWindow::on_toolButton_11_clicked()
{
    QString fullname=ui->lineEdit_search->text();    
   if(fullname=="")
   {
       employee().show_notification("Search","search field is empty");
   }
   else
   {
       ui->empTable->setModel(tableEmployee.search(fullname));
      int n= ui->empTable->model()->rowCount();
      if(n==0)
      {
          employee().show_notification("Search","employee not found \n verify the name");
      }
   }
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

//Pdf creation ( to change )

void MainWindow::on_toolButton_pdf_2_clicked()
{
    ;
    QString filter = "pdf (*.pdf) ";    
    QString code = ui->lineEdit->text();    
    if(code == "")
    {
        employee().show_notification("error","choose an employee to export");
    }
    else
    {
        QString file = QFileDialog::getSaveFileName(this, "save in", QDir::homePath(),filter);
        employee().pdfunction(code,file);
        employee().show_notification("PDF creation","File created successfully");
    }
}

//END EMPLOYEE

//START SHIFTS

//FUNCTIONS

//show employee id + add shifts

void MainWindow::on_toolButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->emptable_2->setModel(tableEmployee1.show_shifts());
}

void MainWindow::on_emptable_2_clicked(const QModelIndex &index)
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

void MainWindow::on_toolButton_save_3_clicked()
{ 
    QString cin= ui->lineEdit_cin_shifts->text();
    QString startday= ui->calendarWidget_shifts->selectedDate().toString();
    QString endday= ui->calendarWidget_shifts_3->selectedDate().toString();
    QString hstart=ui->timeEdit_hdeb->time().toString();
    QString hend=ui->timeEdit_hfin->time().toString();

    int x=0;
    if(ui->lineEdit_cin_shifts->text().isEmpty())
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("Please select an Employee"), QMessageBox::Ok);
        x++;
    }
    else if(ui->calendarWidget_shifts->selectedDate()<=QDate().currentDate())
    {
        QMessageBox::critical(nullptr, QObject::tr("Start date"),
                          QObject::tr("Start date cannot be before current Date"), QMessageBox::Ok);
        x++;
    }
    else if(ui->calendarWidget_shifts->selectedDate() >= ui->calendarWidget_shifts_3->selectedDate())
    {
        QMessageBox::critical(nullptr, QObject::tr("End date"),
                          QObject::tr("End date cannot be lower than Start date"), QMessageBox::Ok);
        x++;
    }
    else if(ui->timeEdit_hdeb->time().toString() < "08:00:00")
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Shifts don't start before 8AM"), QMessageBox::Ok);
        x++;
    }
    else if(ui->timeEdit_hfin->time().toString() > "22:00:00")
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Shifts end at 10PM"), QMessageBox::Ok);
        x++;
    }
    else if(x==0)
    {
        shifts s(cin,startday,endday,hstart,hend);
        bool toTest = s.add();
        if(toTest)
        {
            shifts().show_notification("Shifts","employee's shifts added successfully");
        }
        ui->timeEdit_hdeb->clear();
        ui->timeEdit_hfin->clear();
        ui->stackedWidget->setCurrentIndex(5);
    }
}

//show employee + shifts

void MainWindow::on_toolButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->shiftsTab->setModel(tableShifts.show());
}

//delete shifts

void MainWindow::on_shiftsTab_clicked(const QModelIndex &index)
{
    QString val=ui->shiftsTab->model()->data(index).toString();
    QSqlQuery query;
    query.prepare("SELECT CIN FROM SHIFTS WHERE CIN = '"+val+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->lineEdit_toDo->setText(query.value(0).toString());
        }
    }
}

void MainWindow::on_toolButton_delete_4_clicked()
{
    QString cin = ui->lineEdit_toDo->text();
    tableShifts.remove(cin);
    ui->shiftsTab->setModel(tableShifts.show());
    ui->lineEdit_toDo->clear();
}

// modify shifts

void MainWindow::on_shiftsTab_doubleClicked(const QModelIndex &index)
{
    QString val=ui->shiftsTab->model()->data(index).toString();
    QSqlQuery query;
    query.prepare("SELECT * FROM SHIFTS WHERE CIN = '"+val+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->stackedWidget->setCurrentIndex(8);
            ui->lineEdit_toDo_2->setText(query.value(0).toString());
            ui->calendarWidget_shifts_edit->setSelectedDate(query.value(1).toDate());
            ui->calendarWidget_shifts_edit_2->setSelectedDate(query.value(2).toDate());
            ui->timeEdit_hdeb_edit->setTime(query.value(3).toTime());
            ui->timeEdit_hfin_edit->setTime(query.value(4).toTime());
        }
    }
}

void MainWindow::on_toolButton_edit_6_clicked()
{
    QString cin= ui->lineEdit_toDo_2->text();
    QString startday= ui->calendarWidget_shifts_edit->selectedDate().toString();
    QString endday= ui->calendarWidget_shifts_edit_2->selectedDate().toString();
    QString hstart=ui->timeEdit_hdeb_edit->time().toString();
    QString hend=ui->timeEdit_hfin_edit->time().toString();

    int x=0;
    if(ui->lineEdit_toDo_2->text().isEmpty())
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("Please select an Employee"), QMessageBox::Ok);
        x++;
    }
    else if(ui->calendarWidget_shifts_edit->selectedDate()<=QDate().currentDate())
    {
        QMessageBox::critical(nullptr, QObject::tr("Start date"),
                          QObject::tr("Start date cannot be before current Date"), QMessageBox::Ok);
        x++;
    }
    else if(ui->calendarWidget_shifts_edit->selectedDate() >= ui->calendarWidget_shifts_edit_2->selectedDate())
    {
        QMessageBox::critical(nullptr, QObject::tr("End date"),
                          QObject::tr("End date cannot be lower than Start date"), QMessageBox::Ok);
        x++;
    }
    else if(ui->timeEdit_hdeb_edit->time().toString() < "08:00:00")
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Shifts don't start before 8AM"), QMessageBox::Ok);
        x++;
    }
    else if(ui->timeEdit_hfin_edit->time().toString() > "22:00:00")
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Shifts end at 10PM"), QMessageBox::Ok);
        x++;
    }else if(x==0)
    {
        shifts s(cin,startday,endday,hstart,hend);
        bool toTest=s.modify();
        if(toTest)
        {
            shifts().show_notification("Shifts info","Employee's info changed successfully");
            ui->stackedWidget->setCurrentIndex(5);
        }
    }
}

//ADVANCED FUNCTIONS

//search

void MainWindow::on_toolButton_delete_5_clicked()
{
    QString startday=ui->dateEdit->date().toString();
    if(startday=="")
    {
        employee().show_notification("Search","search field is empty");
    }
    else
    {
        ui->shiftsTab->setModel(tableShifts.search(startday));
       int n= ui->empTable->model()->rowCount();
       if(n==0)
       {
           employee().show_notification("Search","shifts not found \n verify the name");
       }
    }
}

//sort Asc

void MainWindow::on_toolButton_delete_6_clicked()
{
    ui->shiftsTab->setModel(tableShifts.show_Asc());
}

//sort Desc

void MainWindow::on_toolButton_delete_7_clicked()
{
    ui->shiftsTab->setModel(tableShifts.show_Desc());
}

//END SHIFTS

