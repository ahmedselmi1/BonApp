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
#include <QPropertyAnimation>
#include <QTimer>
#include<math.h>
#include <QMediaPlayer>
using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std;


QSound audio (":/img/img/click.wav");

QMediaPlayer* musicPlayer = new QMediaPlayer;
QString songsList[] = {"music1.mp3", "music2.mp3", "music3.mp3", "music4.mp3", "music5.mp3"};
int currentSong = 0;
bool autoPlay = true;
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





     if(autoPlay)
     {

         musicPlayer->setMedia(QUrl("qrc:/music/" + songsList[currentSong]));
         musicPlayer->setVolume(50);
         musicPlayer->play();
         ui->musicStop->setVisible(true);
         ui->musicPlay->setVisible(false);


     }
     else
     {
         ui->musicStop->setVisible(false);
         ui->musicPlay->setVisible(true);

     }

     startTimer(1000);   // 1-second timer















}



void MainWindow::onMsg()
{
    while(A.getserial()->canReadLine())
    {
        data=A.getserial()->readLine();
        data.chop(2);
        qDebug()<<"Data Received: " <<data;
        if(data.split(':')[0] == "b")
        {
            qDebug()<<"button state" << data.split(':')[1];
        }
        else if(data.split(':')[0] == "p")
        {
            qDebug()<<"distance: "<< data.split(':')[1];
            int rawDistance = data.split(':')[1].toInt();
            if(lastPlateValues.size() == 10)
            {
                lastPlateValues.pop_front();
            }

            lastPlateValues.push_back(rawDistance);
            int total = 0;
            int presentNum = 0;
            int totalPresent = 0;
            for(int s=0;s<lastPlateValues.size();s++)
            {
                total += lastPlateValues[s];
                if(lastPlateValues[s] < 100)
                {
                    presentNum++;
                    totalPresent += lastPlateValues[s];
                }
            }

            int distance = total/lastPlateValues.size();
            if(presentNum == 0) presentNum = 1;
            int distancePresent = totalPresent/presentNum;
            if(distance > 100)
            {
                ui->plate->setVisible(false);
                ui->okplate->setVisible(false);
                lastPlateValues.clear();
            }
            else
            {
                ui->plate->setVisible(true);
                int finalDist = max(0,distancePresent-6);
                ui->plate->move(465,290-finalDist*4);
                if(finalDist == 0)
                {
                    ui->okplate->setVisible(true);

                }
                else
                {
                    ui->okplate->setVisible(false);

                }
            }

        }
        else if(data.split(':')[0] == "s")
        {
            if(data.split(':')[1] == "-1")
            {
                ui->label_86->setText("Not Detected"); // si les données reçues de arduino via la liaison série sont égales à 1

            }
            else
            ui->label_86->setText("Detected"); // si les données reçues de arduino via la liaison série sont égales à 1
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
    ui->stackedWidget->setCurrentIndex(20);
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

void MainWindow::on_back_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
}

void MainWindow::on_back_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
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

    ui->stackedWidget->setCurrentIndex(28);

}

void MainWindow::on_ajouterclient_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(27);

}

void MainWindow::on_backbtn_3_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(25);

}

void MainWindow::on_backbtn_4_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(26);

}



void MainWindow::on_gestionClient_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(25);
    lastClientCell = make_pair(-1,-1);
    clients::processClientTable(ui->clientsTable);




}

void MainWindow::on_gestionCoupon_clicked()
{
    QSound::play(":/clicky.wav");

    ui->stackedWidget->setCurrentIndex(26);

    lastCouponsCell = make_pair(-1,-1);

    coupons::processCouponTable(ui->couponsTable, ui->randomLayout);



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

    coupons::deleteSelectedCoupons(ui->couponsTable, ui->randomLayout);

}

void MainWindow::animateObj(QWidget* widget, int current)
{
    if(current == -1 )return;
widget->setStyleSheet("background-color: rgb(255, 0, 0,"+ QString::number(current) + ");");

    QTimer* timer = new QTimer();
    timer->setInterval(1); // set the time in milliseconds
    timer->setSingleShot(true); // in-case if setTimout and false in-case of setInterval

    connect(timer, &QTimer::timeout, [this,widget,current](){
        animateObj(widget, current-1);
    });
    timer->start();

}




void MainWindow::animateObjWhite(QWidget* widget, int current)
{
    if(current == -1 )return;
widget->setStyleSheet("background-color: rgb(255, "+QString::number(255-current)+", "+QString::number(255-current)+",255);");

    QTimer* timer = new QTimer();
    timer->setInterval(1); // set the time in milliseconds
    timer->setSingleShot(true); // in-case if setTimout and false in-case of setInterval

    connect(timer, &QTimer::timeout, [this,widget,current](){
        animateObjWhite(widget, current-1);
    });
    timer->start();

}
void MainWindow::on_ajouterClientBtn_clicked()
{
    QSound::play(":/clicky.wav");


    if(ui->labelClientNom->text() == "")
    {

        QPropertyAnimation* anim = new QPropertyAnimation(ui->nomWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->nomWarning->geometry().x()+5,ui->nomWarning->geometry().y(),ui->nomWarning->geometry().width(),ui->nomWarning->geometry().height()));
        anim->setEndValue(QRect(ui->nomWarning->geometry().x(),ui->nomWarning->geometry().y(),ui->nomWarning->geometry().width(),ui->nomWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelClientNom,255);
        ui->nomWarning->setText("<font color='red'>Nom est vide</font>");
        return;
    }
    else
    {
        ui->nomWarning->setText("");
    }



    if(ui->labelClientPrenom->text() == "")
    {

        QPropertyAnimation* anim = new QPropertyAnimation(ui->prenomWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->prenomWarning->geometry().x()+5,ui->prenomWarning->geometry().y(),ui->prenomWarning->geometry().width(),ui->prenomWarning->geometry().height()));
        anim->setEndValue(QRect(ui->prenomWarning->geometry().x(),ui->prenomWarning->geometry().y(),ui->prenomWarning->geometry().width(),ui->prenomWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelClientPrenom,255);
        ui->prenomWarning->setText("<font color='red'>Prenom est vide</font>");
        return;
    }
    else
    {
        ui->prenomWarning->setText("");
    }


    if(ui->labelClientAdresse->text() == "")
    {

        QPropertyAnimation* anim = new QPropertyAnimation(ui->adresseWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->adresseWarning->geometry().x()+5,ui->adresseWarning->geometry().y(),ui->adresseWarning->geometry().width(),ui->adresseWarning->geometry().height()));
        anim->setEndValue(QRect(ui->adresseWarning->geometry().x(),ui->adresseWarning->geometry().y(),ui->adresseWarning->geometry().width(),ui->adresseWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelClientAdresse,255);
        ui->adresseWarning->setText("<font color='red'>Adresse est vide</font>");
        return;
    }
    else
    {
        ui->adresseWarning->setText("");
    }


    if(ui->labelClientEmail->text() == "")
    {
        QPropertyAnimation* anim = new QPropertyAnimation(ui->emailWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->emailWarning->geometry().x()+5,ui->emailWarning->geometry().y(),ui->emailWarning->geometry().width(),ui->emailWarning->geometry().height()));
        anim->setEndValue(QRect(ui->emailWarning->geometry().x(),ui->emailWarning->geometry().y(),ui->emailWarning->geometry().width(),ui->emailWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelClientEmail,255);
        ui->emailWarning->setText("<font color='red'>Email est vide</font>");
        return;
    }
    else
    {
        QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
        mailREX.setCaseSensitivity(Qt::CaseInsensitive);
        mailREX.setPatternSyntax(QRegExp::RegExp);
        qDebug() << mailREX.exactMatch("me@me.com");

        if(!mailREX.exactMatch(ui->labelClientEmail->text()))
        {

            QPropertyAnimation* anim = new QPropertyAnimation(ui->emailWarning, "geometry");
            anim->setDuration(500);
            anim->setEasingCurve(QEasingCurve::InOutSine);
            anim->setStartValue(QRect(ui->emailWarning->geometry().x()+5,ui->emailWarning->geometry().y(),ui->emailWarning->geometry().width(),ui->emailWarning->geometry().height()));
            anim->setEndValue(QRect(ui->emailWarning->geometry().x(),ui->emailWarning->geometry().y(),ui->emailWarning->geometry().width(),ui->emailWarning->geometry().height()));
            anim->start();
            animateObjWhite(ui->labelClientEmail,255);
            ui->emailWarning->setText("<font color='red'>Mauvaise Email</font>");




            return;
        }


        ui->emailWarning->setText("");
    }

    if(ui->labelClientTel->text() == "")
    {

        QPropertyAnimation* anim = new QPropertyAnimation(ui->numWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->numWarning->geometry().x()+5,ui->numWarning->geometry().y(),ui->numWarning->geometry().width(),ui->numWarning->geometry().height()));
        anim->setEndValue(QRect(ui->numWarning->geometry().x(),ui->numWarning->geometry().y(),ui->numWarning->geometry().width(),ui->numWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelClientTel,255);
        ui->numWarning->setText("<font color='red'>Tel est vide</font>");
        return;
    }
    else
    {
        ui->numWarning->setText("");
    }




    bool added = clients::addClientToDB(ui->labelClientNom->text(),ui->labelClientPrenom->text(), ui->labelClientAdresse->text(),ui->labelClientEmail->text(),ui->labelClientTel->text());

    if(added)
    {
        clients::processClientTable(ui->clientsTable);
        QMessageBox::information(nullptr, QObject::tr("Client Added"),QObject::tr("Client Added Successfuly"), QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(25);
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

    bool bad = false;
    if(ui->labelCouponCode->text() == "")
    {

        QPropertyAnimation* anim = new QPropertyAnimation(ui->codeWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->codeWarning->geometry().x()+5,ui->codeWarning->geometry().y(),ui->codeWarning->geometry().width(),ui->codeWarning->geometry().height()));
        anim->setEndValue(QRect(ui->codeWarning->geometry().x(),ui->codeWarning->geometry().y(),ui->codeWarning->geometry().width(),ui->codeWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelCouponCode,255);
        ui->codeWarning->setText("<font color='red'>Code est vide</font>");
        bad = true;
    }
    else
    {
        ui->codeWarning->setText("");
    }

    if(ui->labelCouponUses->text() == "")
    {

        QPropertyAnimation* anim = new QPropertyAnimation(ui->usesWarning, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setStartValue(QRect(ui->usesWarning->geometry().x()+5,ui->usesWarning->geometry().y(),ui->usesWarning->geometry().width(),ui->usesWarning->geometry().height()));
        anim->setEndValue(QRect(ui->usesWarning->geometry().x(),ui->usesWarning->geometry().y(),ui->usesWarning->geometry().width(),ui->usesWarning->geometry().height()));
        anim->start();
        animateObjWhite(ui->labelCouponUses,255);
        ui->usesWarning->setText("<font color='red'>Uses est vide</font>");
        bad = true;
    }
    else
    {
        for(int s=0;s<ui->labelCouponUses->text().size();s++)
        {
            if(!ui->labelCouponUses->text()[s].isDigit())
            {
                QPropertyAnimation* anim = new QPropertyAnimation(ui->usesWarning, "geometry");
                anim->setDuration(500);
                anim->setEasingCurve(QEasingCurve::InOutSine);
                anim->setStartValue(QRect(ui->usesWarning->geometry().x()+5,ui->usesWarning->geometry().y(),ui->usesWarning->geometry().width(),ui->usesWarning->geometry().height()));
                anim->setEndValue(QRect(ui->usesWarning->geometry().x(),ui->usesWarning->geometry().y(),ui->usesWarning->geometry().width(),ui->usesWarning->geometry().height()));
                anim->start();
                animateObjWhite(ui->labelCouponUses,255);
                ui->usesWarning->setText("<font color='red'>Uses est un nombre</font>");
                bad = true;
                break;
            }
        }
        if(!bad)ui->usesWarning->setText("");
    }

    if(bad)
    {
        return;
    }


    QSound::play(":/clicky.wav");

    bool added = coupons::addCouponToDB(ui->labelCouponCode->text(),ui->labelCouponUses->text(), ui->CouponCalendarStart->selectedDate().toString("yyyy-MM-dd") + " " + ui->CouponTimeStart->time().toString("hh:mm:ss"),ui->CouponCalendarEnd->selectedDate().toString("yyyy-MM-dd") + " " + ui->CouponTimeEnd->time().toString("hh:mm:ss"),'{' + (ui->couponTotalSup->isChecked()?("\"totalSup\": " + ui->couponTotalSupInput->text()):"") +  '}', ui->notifEmailCoupon->isChecked());

    if(added)
    {
        coupons::processCouponTable(ui->couponsTable,  ui->randomLayout);
        QMessageBox::information(nullptr, QObject::tr("Coupon Added"),QObject::tr("Coupon Added Successfuly"), QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(26);
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

void MainWindow::on_backvitrine_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}





////////////////////////////////////GHADA////////////////////////////////////////








//Gateaux part

void MainWindow::on_toolButton_save_clicked()
{
    QString ID= ui->lineEdit_id->text();
    QString NAME= ui->lineEdit_name->text();
    int QUANTITY= ui->spinBox_quantity->text().toInt();
    int PRICE= ui->lineEdit_price->text().toInt();
    QString TYPE= ui->lineEdit_type->text();


        int x=0;

        if(QUANTITY<=0)
        {
            x++;
        }
        else if(NAME=="")
        {
            x++;
        }
        else if(PRICE<=0)
        {
            x++;
        }

        else if (x==0)
        {
            gateauxC g(ID,NAME,QUANTITY,PRICE,TYPE);
            bool toTest=g.add_gateaux();

        if(toTest)
        {
            gateauxC().show_notification("Add cake","Cake added successfully");
        }
        ui->lineEdit_name->clear();
        ui->lineEdit_id->clear();
        ui->spinBox_quantity->clear();
        ui->lineEdit_price->clear();
        ui->stackedWidget->setCurrentIndex(20);
        }

}

void MainWindow::on_toolButton_delete_clicked()
{
    QString type=ui->type->text();
    int id = ui->comboBox_id->currentText().toInt();
    tabGateaux.remove(id);
    ui->GatTab->setModel(tabGateaux.show(type));
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from GATEAUX"));
    ui->comboBox_id->setModel(mod);
    audio.play();
}



void MainWindow::on_toolButton_savemod_clicked()
{
    QString ID = ui->lineEdit_id_3->text();
    QString NAME= ui->lineEdit_name_3->text();
    int QUANTITY= ui->spinBox_quantity_3->text().toInt();
    int PRICE= ui->lineEdit_price_3->text().toInt();
    QString TYPE=ui->lineEdit_type_3->text();

    int x=0;
    if (x==0)
    {
     gateauxC g(ID,NAME,QUANTITY,PRICE,TYPE);
    bool toTest =g.modify();
    if(toTest)
    {
         gateauxC().show_notification("Modification cake","Cake modified successfully");
    }
    ui->lineEdit_name_3->clear();
    ui->spinBox_quantity_3->clear();
    ui->lineEdit_price_3->clear();
    ui->lineEdit_type_3->clear();
    ui->stackedWidget->setCurrentIndex(4);
    ui->GatTab->setModel(tabGateaux.show(TYPE));
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from GATEAUX"));
    }
}

void MainWindow::on_GatTab_activated(const QModelIndex &index)
{
    QString val=ui->GatTab->model()->data(index).toString();
        QSqlQuery query;
        query.prepare("SELECT * FROM GATEAUX WHERE ID = '"+val+"'");
        if(query.exec())
        {
            while (query.next())
            {
                ui->stackedWidget->setCurrentIndex(15);
                ui->lineEdit_id_3->setText(query.value(0).toString());
                ui->lineEdit_name_3->setText(query.value(1).toString());
                ui->spinBox_quantity_3->setValue(query.value(2).toInt());
                ui->lineEdit_price_3->setText(query.value(3).toString());
                ui->lineEdit_type_3->setText(query.value(4).toString());
            }
        }
}
//COMMANDE

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select NAME from GATEAUX"));
    ui->comboBox_name->setModel(mod);
    audio.play();
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->tableC->setModel(tablecommande.showc());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from COMMANDE"));
    ui->comboBox_3->setModel(mod);
    audio.play();

}

void MainWindow::on_pushButton_3_clicked()
{
    QString ID= ui->lineEdit_idc->text();
    QString GATEAUXNAME=ui->comboBox_name->currentText();
    int SOMME= ui->lineEdit_somme->text().toInt();
    int QUANTC= ui->lineEdit_quantc->text().toInt();
    int q= ui->lineEdit_quantc_2->text().toInt();



        int x=0;
        if(ID=="")
        {
            QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                        QObject::tr("please enter an ID"), QMessageBox::Ok);
                  x++;
        }
        else if(QUANTC>q)
        {
            QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                        QObject::tr("Check the quantity"), QMessageBox::Ok);
                  x++;
        }
        else if(QUANTC>q)
        {
            QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                        QObject::tr("Check the quantity"), QMessageBox::Ok);
                  x++;
        }

        else if (x==0)
        {
            commandeC cc(ID,GATEAUXNAME,SOMME,QUANTC);
            bool toTest=cc.add_commande();

        if(toTest)
        {
             commandeC().show_notification("Add order","Order added successfully");
        }
        ui->lineEdit_idc->clear();
        ui->lineEdit_somme->clear();
        ui->comboBox_name->clear();
        ui->lineEdit_quantc->clear();
        ui->stackedWidget->setCurrentIndex(1);
        }
}

void MainWindow::on_tableC_activated(const QModelIndex &index)
{
    QString val=ui->tableC->model()->data(index).toString();
        QSqlQuery query;
        query.prepare("SELECT * FROM COMMANDE WHERE ID = '"+val+"'");
        if(query.exec())
        {
            while (query.next())
            {
                ui->stackedWidget->setCurrentIndex(18);
                ui->lineEdit_idc_1->setText(query.value(0).toString());
                ui->comboBox_name_1->setEditText(query.value(1).toString());
                ui->lineEdit_total_1->setText(query.value(2).toString());
                ui->lineEdit_quantc_1->setText(query.value(3).toString());
                QSqlQueryModel *mod= new QSqlQueryModel();
                mod->setQuery(("select NAME from GATEAUX"));
                ui->comboBox_name_1->setModel(mod);
            }
        }
}

void MainWindow::on_pushButton_deletec_clicked()
{
    int id = ui->comboBox_3->currentText().toInt();
    tablecommande.removec(id);
    ui->tableC->setModel(tablecommande.showc());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from COMMANDE"));
    ui->comboBox_3->setModel(mod);
    audio.play();
}

void MainWindow::on_comboBox_name_currentIndexChanged(const QString &arg1)
{
    QString val=ui->comboBox_name->currentText();
        QSqlQuery query;
        query.prepare("SELECT PRICE, QUANTITY FROM GATEAUX WHERE NAME = '"+val+"'");
        if(query.exec())
        {
            while (query.next())
            {
                ui->lineEdit_setPrice->setText(query.value(0).toString());
                ui->lineEdit_quantc_2->setText(query.value(1).toString());
            }
        }
}

void MainWindow::on_pushButton_4_clicked()
{
    int price = ui->lineEdit_setPrice->text().toInt();
    int nb = ui->lineEdit_quantc->text().toInt();
    int total = price * nb;
    QString t = QString::number(total);
    ui->lineEdit_somme->setText(t);
    audio.play();
}

void MainWindow::on_pushButton_savemod_clicked()
{
    QString ID= ui->lineEdit_idc_1->text();
    QString GATEAUXNAME=ui->comboBox_name_1->currentText();
    int SOMME= ui->lineEdit_total_1->text().toInt();
    int QUANTC= ui->lineEdit_quantc_1->text().toInt();
    //int q= ui->lineEdit_quan_1->text().toInt();

    int x=0;
    if (x==0)
    {
     commandeC cc(ID,GATEAUXNAME,SOMME,QUANTC);
    bool toTest =cc.modifyc();
    if(toTest)
    {
         commandeC().show_notification("Add order","Order modified successfully");
    }
    ui->comboBox_name_1->clear();
    ui->lineEdit_quantc_1->clear();
    ui->lineEdit_total_1->clear();
    ui->stackedWidget->setCurrentIndex(17);
    ui->tableC->setModel(tablecommande.showc());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from COMMANDE"));
    }
}

void MainWindow::on_pushButton_calc_clicked()
{
    int price = ui->lineEdit_setprice_1->text().toInt();
    int nb = ui->lineEdit_quantc_1->text().toInt();
    int total = price * nb;
    QString t = QString::number(total);
    ui->lineEdit_total_1->setText(t);
    audio.play();
}

void MainWindow::on_comboBox_name_1_currentIndexChanged(const QString &arg1)
{
    QString val=ui->comboBox_name_1->currentText();
        QSqlQuery query;
        query.prepare("SELECT PRICE, QUANTITY FROM GATEAUX WHERE NAME = '"+val+"'");
        if(query.exec())
        {
            while (query.next())
            {
                ui->lineEdit_setprice_1->setText(query.value(0).toString());
                ui->lineEdit_quan_1->setText(query.value(1).toString());
            }
        }
}


void MainWindow::on_pushButton_search_2_clicked()
{
    QString name=ui->lineEdit_search_2->text();
        ui->tableC->setModel(tablecommande.search(name));
        audio.play();
}

void MainWindow::on_pushButton_search_clicked()
{
    QString name=ui->lineEdit_search->text();
        ui->GatTab->setModel(tabGateaux.search(name));
        audio.play();
}

void MainWindow::on_pushButton_sort_2_clicked()
{
    ui->tableC->setModel(tablecommande.show_Asc());
    audio.play();
}

void MainWindow::on_pushButton_sort_clicked()
{
    ui->GatTab->setModel(tabGateaux.show_Desc());
    audio.play();
}

void MainWindow::on_pushButton_pdf_clicked()
{
    QString filter = "pdf (*.pdf) ";
    QString file = QFileDialog::getSaveFileName(this, "save in", QDir::homePath(),filter);
    /*gateauxC().pdfunction(file);*/
    audio.play();
}

void MainWindow::on_pushButton_ex_clicked()
{
    QTableView *table;
                   table = ui->GatTab;
                   QString filters("Excel  (.xlsx);;All files (.)");
                   QString defaultFilter("Excel (*.xlsx)");
                   QString fileName = QFileDialog::getSaveFileName(0, "Save file", QCoreApplication::applicationDirPath(),
                                      filters, &defaultFilter);
                   QFile file(fileName);
                   QAbstractItemModel *model =  table->model();
                   if (file.open(QFile::WriteOnly | QFile::Truncate)) {
                       QTextStream data(&file);
                       QStringList strList;
                       for (int i = 0; i < model->columnCount(); i++) {
                           if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                               strList.append(""" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + """);
                           else
                               strList.append("");
                       }
                       data << strList.join(";") << "\n";
                       for (int i = 0; i < model->rowCount(); i++) {
                           strList.clear();
                           for (int j = 0; j < model->columnCount(); j++) {
                               if (model->data(model->index(i, j)).toString().length() > 0)
                                   strList.append(""" + model->data(model->index(i, j)).toString() + """);
                               else
                                   strList.append("");
                           }
                           data << strList.join(";") + "\n";
                       }
                       file.close();
                       QMessageBox::information(this,"Exporter To Excel","Exporter En Excel Avec Succées ");
                   }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString code = ui->lineEdit_bill->text();
    QString filter = "pdf (*.pdf) ";
    QString file = QFileDialog::getSaveFileName(this, "save in", QDir::homePath(),filter);
    commandeC().pdfunction(code,file);
    audio.play();
}

void MainWindow::on_pushButton_stat_clicked()
{   int stat1,stat2,stat3;

    gateauxC().stat_1(stat1);
    gateauxC().stat_2(stat2);
    gateauxC().stat_3(stat3);
    QPieSeries *series = new QPieSeries();

    series->append("price lower than 20",stat1);
    series->append("price between 20 and 50",stat2);
    series->append("price higher than 50",stat3);

    series->setHoleSize(0.5);
    series->setPieSize(0.8);

    QPieSlice *first = series->slices().at(0);
    QPieSlice *second = series->slices().at(1);
    QPieSlice *third = series->slices().at(2);

    first->setLabelVisible(true);
    second->setLabelVisible(true);
    third->setLabelVisible(true);

    first->setBrush(Qt::red);
    second->setBrush(Qt::yellow);
    third->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("pie chart");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView * chartview = new QChartView(chart);
    chartview->setParent(ui->stat_c);
    ui->stackedWidget->setCurrentIndex(19);
    audio.play();
}

void MainWindow::on_back_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_back_16_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_18_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
}
//CATEGORY PART

void MainWindow::on_pushButton_6_clicked()
{
    QString id= ui->lineEdit->text();
    QString type= ui->lineEdit_2->text();


        int x=0;

        if(type=="")
        {
            x++;
        }


        else if (x==0)
        {
            category cat(id,type);
            bool toTest=cat.add_category();

        if(toTest)
        {
            category().show_notification("Add category","Category added successfully");
        }
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->stackedWidget->setCurrentIndex(20);
        }

}

void MainWindow::on_pushButton_7_clicked()
{
    int id = ui->comboBox_4->currentText().toInt();
    tabCategory.remove(id);
    ui->CatTab->setModel(tabCategory.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from CATEGORY"));
    ui->comboBox_4->setModel(mod);
    audio.play();
}

void MainWindow::on_pushButton_8_clicked()
{
    QString name=ui->lineEdit_3->text();
        ui->CatTab->setModel(tabCategory.search(name));
        audio.play();
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->CatTab->setModel(tabCategory.show_Desc());
    audio.play();
}

void MainWindow::on_addCategory_clicked()
{
    ui->stackedWidget->setCurrentIndex(21);
    audio.play();
}

void MainWindow::on_showCategory_clicked()
{
    ui->stackedWidget->setCurrentIndex(22);
    ui->CatTab->setModel(tabCategory.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from CATEGORY"));
    ui->comboBox_4->setModel(mod);
    audio.play();
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    audio.play();
}

void MainWindow::on_CatTab_activated(const QModelIndex &index)
{
    QString val=ui->CatTab->model()->data(index).toString();
        QSqlQuery query;
        query.prepare("SELECT * FROM CATEGORY WHERE TYPE = '"+val+"'");
        if(query.exec())
        {
            while (query.next())
            {
                ui->stackedWidget->setCurrentIndex(4);
                ui->GatTab->setModel(tabGateaux.show(val));
                ui->type->setText(val);
            }
        }
        QString valm=ui->CatTab->model()->data(index).toString();
            QSqlQuery queryy;
            queryy.prepare("SELECT * FROM CATEGORY WHERE ID = '"+valm+"'");
            if(queryy.exec())
            {
                while (queryy.next())
                {
                    ui->stackedWidget->setCurrentIndex(23);
                    ui->lineEdit_4->setText(queryy.value(0).toString());
                    ui->lineEdit_5->setText(queryy.value(1).toString());

                }

            }
}

void MainWindow::on_back_20_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    audio.play();
}

void MainWindow::on_back_22_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    audio.play();
}

void MainWindow::on_pushButton_11_clicked()
{
    QString id = ui->lineEdit_4->text();
    QString type= ui->lineEdit_5->text();

    int x=0;
    if (x==0)
    {
     category cat(id,type);
    bool toTest =cat.modify();
    if(toTest)
    {
         category().show_notification("Modification cake","Cake modified successfully");
    }
    ui->lineEdit_5->clear();
    ui->stackedWidget->setCurrentIndex(22);
    ui->CatTab->setModel(tabCategory.show());
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from CATEGORY"));
    }
}


void MainWindow::on_pushButton_12_clicked()
{
     ui->stackedWidget->setCurrentIndex(24);
     audio.play();
}

void MainWindow::on_showProducts_clicked()
{
    QString type=ui->type->text();
    ui->stackedWidget->setCurrentIndex(4);
    ui->GatTab->setModel(tabGateaux.show(type));
    QSqlQueryModel *mod= new QSqlQueryModel();
    mod->setQuery(("select ID from GATEAUX"));
    ui->comboBox_id->setModel(mod);
    audio.play();
}



void MainWindow::on_vitrineBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(29);
    audio.play();

}

void MainWindow::on_musicNext_clicked()
{
    int length = sizeof(songsList) / sizeof(songsList[0]);
    currentSong = (currentSong + 1)%length;
    musicPlayer->setMedia(QUrl("qrc:/music/" + songsList[currentSong]));
    musicPlayer->play();

}

void MainWindow::on_musicBack_clicked()
{
    int length = sizeof(songsList) / sizeof(songsList[0]);

    currentSong = (currentSong - 1);
    if(currentSong<0)
    {
        currentSong = length-1;
    }
    musicPlayer->setMedia(QUrl("qrc:/music/" + songsList[currentSong]));
    musicPlayer->play();

}

void MainWindow::on_musicStop_clicked()
{
    musicPlayer->pause();
    ui->musicStop->setVisible(false);
    ui->musicPlay->setVisible(true);
}

void MainWindow::on_musicPlay_clicked()
{
    musicPlayer->play();
    ui->musicStop->setVisible(true);
    ui->musicPlay->setVisible(false);

}
void MainWindow::timerEvent(QTimerEvent * event)
{
    ui->timeTXT->setText("<span style=' font-size:22pt; color:#ffffff;'>" + QTime::currentTime().toString("hh:mm:ss") + "</span>");
}
