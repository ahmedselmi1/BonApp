#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "coupons.h"
#include <QDebug>
#include <QApplication>
#include <QSqlRecord>
#include <QMessageBox>
#include <iostream>
#include<string>
#include <algorithm>
#include <vector>
#include <map>
#include "QrCode.hpp"
#include <QPainter>
#include <QDebug>
#include "qrcodedisplayer.h"
#include <QPrinter>
#include <QSound>
using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    auto header = ui->clientsTable->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, [this](int logicalIndex){
        clients::sortAccording(ui->clientsTable, logicalIndex);
    });


    auto header2 = ui->couponsTable->horizontalHeader();
    connect(header2, &QHeaderView::sectionClicked, [this](int logicalIndex){
        clients::sortAccording(ui->couponsTable, logicalIndex);
    });



    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(onMsg())); // permet de lancer


}



void MainWindow::onMsg()
{
    while(A.getserial()->canReadLine())
    {
        data=A.getserial()->readLine();
        qDebug()<<"Data Received: " <<data;
        if(data.split(':')[0] == "b")
        {
            qDebug()<<"very cool";
        }
    }



}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_goToOrders_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}








void MainWindow::paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg)
{
    // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);
    const int s=qr.getSize()>0?qr.getSize():1;
    const double w=sz.width();
    const double h=sz.height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);
    // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
    // It expects background to be prepared already (in white or whatever is preferred).
    painter.setPen(Qt::NoPen);
    painter.setBrush(fg);
    for(int y=0; y<s; y++) {
        for(int x=0; x<s; x++) {
            const int color=qr.getModule(x, y);  // 0 for white, 1 for black
            if(0!=color) {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painter.drawRects(&r,1);
            }
        }
    }
}


void MainWindow::on_goToProducts_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_goToStock_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_goToClient_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);

    QSound::play(":/clicky.wav");

}

void MainWindow::on_back_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_back_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_back_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_back_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_back_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_addProduct_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_showProducts_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_back_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_back_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_back_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_back_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_addProduct_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_showProducts_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void MainWindow::on_back_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_back_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_toolButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
}

void MainWindow::on_toolButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
}

void MainWindow::on_back_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_back_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

//START EMPLOYEE PART

void MainWindow::on_goToEmployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_toolButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_toolButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select CIN from EMPLOYEE"));
    ui->comboBox_2->setModel(mod);
}

void MainWindow::on_back_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

//show employees

void MainWindow::on_toolButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
    ui->empTable->setModel(tableEmployee.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select CIN from EMPLOYEE"));
    ui->comboBox->setModel(mod);
}

//add employee

void MainWindow::on_toolButton_save_2_clicked()
{
    int cin= ui->lineEdit_cin->text().toInt();
    QString fullname= ui->lineEdit_fullname->text();
    int age= ui->lineEdit_age->text().toInt();
    QString email= ui->lineEdit_email->text();
    int phone= ui->lineEdit_phone->text().toInt();
    QString address= ui->lineEdit_address->text();

    int x=0;

    string str_cin = to_string(cin);
    string str_phone = to_string(phone);
    string m = email.toStdString();

    if (str_cin.length()!=8)
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
    ui->stackedWidget->setCurrentIndex(7);
    }
}

//delete employee

void MainWindow::on_toolButton_10_clicked()
{
    int cin = ui->comboBox->currentText().toInt();
    tableEmployee.remove(cin);
    ui->empTable->setModel(tableEmployee.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select CIN from EMPLOYEE"));
    ui->comboBox->setModel(mod);
}

//modify employee

void MainWindow::on_toolButton_modify_clicked()
{
    int cin = ui->comboBox_2->currentText().toInt();
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
    ui->stackedWidget->setCurrentIndex(7);
    }
}

//END EMPLOYEE PART

void MainWindow::on_backbtn_2_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(6);


}

void MainWindow::on_backbtn_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(6);
    ui->userSearch->clear();

}

void MainWindow::on_ajoutercoupon_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(18);

}

void MainWindow::on_ajouterclient_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(17);

}

void MainWindow::on_backbtn_3_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(15);

}

void MainWindow::on_backbtn_4_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(16);

}



void MainWindow::on_pushButton_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(15);
    lastClientCell = make_pair(-1,-1);
    clients::processClientTable(ui->clientsTable);




}

void MainWindow::on_pushButton_2_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(16);

    lastCouponsCell = make_pair(-1,-1);

    coupons::processCouponTable(ui->couponsTable);



/*

    qrcodedisplayer* qrDisplayer = new qrcodedisplayer;
    qrDisplayer->show();


    */
}

void MainWindow::on_supprimerClient_clicked()
{
    QSound::play(":/clicky.wav");

    clients::deleteSelectedClients(ui->clientsTable);


}

void MainWindow::on_supprimerCoupon_clicked()
{
    QSound::play(":/clicky.wav");

    coupons::deleteSelectedCoupons(ui->couponsTable);

}

void MainWindow::on_ajouterClientBtn_clicked()
{
    QSound::play(":/clicky.wav");

    bool added = clients::addClientToDB(ui->labelClientNom->text(),ui->labelClientPrenom->text(), ui->labelClientAdresse->text(),ui->labelClientEmail->text(),ui->labelClientTel->text());

    if(added)
    {
        clients::processClientTable(ui->clientsTable);
        QMessageBox::information(nullptr, QObject::tr("Client Added"),QObject::tr("Client Added Successfuly"), QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(15);
    }


}


void MainWindow::on_clientsTable_cellChanged(int row, int column)
{
    //clients::changeSelectedClientCell(ui->clientsTable, row, column);
    //QDebug
    //qDebug() <<"Edited "<<row<<" and "<< column;
    if(row == lastClientCell.first && column == lastClientCell.second && column != 0)
    {
        clients::editClientData(ui->clientsTable,row);
        lastClientCell = make_pair(-1,-1);

    }
}



void MainWindow::on_clientsTable_cellDoubleClicked(int row, int column)
{

    lastClientCell = make_pair(row,column);

}

void MainWindow::on_labelClientNom_editingFinished()
{

}

void MainWindow::on_labelClientNom_textEdited(const QString &arg1)
{
    ui->QRCODE->setQRData(arg1);
}

void MainWindow::on_ajoutCoupon_clicked()
{

    QSound::play(":/clicky.wav");

    bool added = coupons::addCouponToDB(ui->labelCouponCode->text(),ui->labelCouponUses->text(), ui->CouponCalendarStart->selectedDate().toString("yyyy-MM-dd") + " " + ui->CouponTimeStart->time().toString("hh:mm:ss"),ui->CouponCalendarEnd->selectedDate().toString("yyyy-MM-dd") + " " + ui->CouponTimeEnd->time().toString("hh:mm:ss"),'{' + (ui->couponTotalSup->isChecked()?("\"totalSup\": " + ui->couponTotalSupInput->text()):"") +  '}', ui->notifEmailCoupon->isChecked());

    if(added)
    {
        coupons::processCouponTable(ui->couponsTable);
        QMessageBox::information(nullptr, QObject::tr("Coupon Added"),QObject::tr("Coupon Added Successfuly"), QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(16);
    }


}

void MainWindow::on_labelCouponCode_textChanged(const QString &arg1)
{
    ui->qrcoupon->setQRData("bonappcoupon;;"+ arg1);
}

void MainWindow::on_couponsTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{

}

void MainWindow::on_couponsTable_cellChanged(int row, int column)
{
    if(row == lastCouponsCell.first && column == lastCouponsCell.second && column != 0)
    {
        coupons::editCouponData(ui->couponsTable,row);
        lastCouponsCell = make_pair(-1,-1);

    }
}

void MainWindow::on_couponsTable_cellDoubleClicked(int row, int column)
{
    lastCouponsCell = make_pair(row,column);

}

void MainWindow::on_printtable1_clicked()
{
    coupons::printPDF(ui->couponsTable);
    QMessageBox::information(nullptr, QObject::tr("PDF Saved"),
                      QObject::tr("PDF file saved as coupons.pdf"), QMessageBox::Ok);

}

void MainWindow::on_printtable2_clicked()
{
    QSound::play(":/clicky.wav");

    clients::printPDF(ui->clientsTable);
    QMessageBox::information(nullptr, QObject::tr("PDF Saved"),
                      QObject::tr("PDF file saved as clients.pdf"), QMessageBox::Ok);

}

void MainWindow::on_userSearch_textChanged()
{
   // ui->clientsTable->items

    clients::searchText(ui->clientsTable,ui->userSearch->toPlainText());

}

void MainWindow::on_clientsTable_cellClicked(int row, int column)
{
    qDebug()<<"i clicked "<<row<< " "<< column;
}

void MainWindow::on_toolButton_5_clicked()
{
    if(ui->userSearch->toPlainText() == "")clients::nextPage(ui->clientsTable);
}

void MainWindow::on_toolButton_6_clicked()
{
    if(ui->userSearch->toPlainText() == "")clients::prevPage(ui->clientsTable);

}


void MainWindow::on_couponSearch_textChanged()
{
    qDebug()<<"huuijohi";

    coupons::searchText(ui->couponsTable,ui->couponSearch->toPlainText());

}


void MainWindow::on_nextCoupon_clicked()
{
    qDebug()<<"hi";
    if(ui->couponSearch->toPlainText() == "")coupons::nextPage(ui->couponsTable);
}

void MainWindow::on_prevCoupon_clicked()
{
    if(ui->couponSearch->toPlainText() == "")coupons::prevPage(ui->couponsTable);
}
