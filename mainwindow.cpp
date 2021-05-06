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
#include"dateformatdelegate.h"


using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std;


QSound audio (":/img/img/click.wav");

QMediaPlayer* musicPlayer = new QMediaPlayer;
QString songsList[] = {"music1.mp3", "music2.mp3", "music3.mp3", "music4.mp3", "music5.mp3"};
int currentSong = 0;
bool autoPlay = false;
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


     ui->stackedWidget->setCurrentIndex(0);

     musicPlayer->setMedia(QUrl("qrc:/music/" + songsList[currentSong]));
     musicPlayer->setVolume(50);


     if(autoPlay)
     {

         musicPlayer->play();
         ui->musicStop->setVisible(true);
         ui->musicPlay->setVisible(false);


     }
     else
     {
         ui->musicStop->setVisible(false);
         ui->musicPlay->setVisible(true);

     }

        // 1-second timer









    //ahmed

     //ui->stackedWidget->setCurrentIndex(0);

     qDebug() << "Starting recipe code";
     tableModel = new QSqlTableModel();
     tableModel->setEditStrategy(QSqlTableModel::OnRowChange);
     tableModel->setTable("RECETTES");
     tableModel->select();
     ui->rafficherTable->setModel(tableModel);
     //ui->rafficherTable->setModel(Rtmp.afficher());
     ui->queueTableView->setModel(Qtmp.afficher());
     qDebug() << "finished recipes code";

     ui->rafficherTable->setColumnHidden(3, true);
     ui->rafficherTable->setItemDelegateForColumn(2, new DateFormatDelegate(this));

     auto headerr = ui->rafficherTable->horizontalHeader();
     connect(headerr, &QHeaderView::sectionClicked, [this](int logicalIndex){
         QString text = ui->rafficherTable->model()->headerData(logicalIndex, Qt::Horizontal).toString();
         isAscending = !isAscending;
         tableModel->sort(logicalIndex, isAscending?Qt::AscendingOrder:Qt::DescendingOrder);
        qDebug() << logicalIndex << text;
     });
     QSound::play(":/startup.wav");

     //fin ahmed


     //start karim

     ui->tableView22->setModel(Mtmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));
     ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
      ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));

     //end karim



startTimer(1000);
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
            if(data.split(':')[1] == "1")safeZonePlate = lastDist;
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
                int finalDist = max(0,distancePresent-safeZonePlate);
                lastDist = distancePresent;
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
    ui->stackedWidget->setCurrentIndex(41);
}

void MainWindow::on_showProducts_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(43);
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

void MainWindow::on_back_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

//show employees

//add employee


//delete employee

//modify employee

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





////////////LOUAY








//+25




void MainWindow::on_back_24_clicked()
{
    ui->stackedWidget->setCurrentIndex(6+3);
}



void MainWindow::on_backToShiftsList_clicked()
{
    ui->stackedWidget->setCurrentIndex(7+25);
}

void MainWindow::on_backToShiftsfcn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5+25);
}

void MainWindow::on_backToShiftsfcn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(5+25);
}

void MainWindow::on_goBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_goToAddEmployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(2+6);
}

void MainWindow::on_backToEmployeeList_clicked()
{
    ui->stackedWidget->setCurrentIndex(3+6);
}

void MainWindow::on_goToShifts_clicked()
{
    ui->stackedWidget->setCurrentIndex(5+25);
}

void MainWindow::on_backToEmployeefnc_clicked()
{
    ui->stackedWidget->setCurrentIndex(1+6);
}

void MainWindow::on_backToEmployeefnc_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1+6);
}

void MainWindow::on_backToEmployeefnc_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1+6);
}

//START EMPLOYEE

//FUNCTIONS

//show all employees

void MainWindow::on_viewEmployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(3+6);
    ui->empTable->setModel(tableEmployee.show());
}

//add employee + validation

void MainWindow::on_addEmployeeToDB_clicked()
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

    else if (age<20 || age>26)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Age must be between 20 and 26"), QMessageBox::Ok);
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
    ui->stackedWidget->setCurrentIndex(7);
    }
}

//delete employee (to change)

void MainWindow::on_empTable_clicked(const QModelIndex &index)
{
    QString val = ui->empTable->model()->data(index).toString();
    ui->lineEdit_6->setText(val);
}

void MainWindow::on_deleteEmployee_clicked()
{
    QString cin = ui->lineEdit_6->text();
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

void MainWindow::on_empTable_activated()
{
            QItemSelectionModel *select = ui->empTable->selectionModel();
            ui->lineEdit_fullname_m_2->setText(select->selectedRows(1).value(0).data().toString());
            ui->lineEdit_age_m_2->setText(select->selectedRows(2).value(0).data().toString());
            ui->lineEdit_email_m_2->setText(select->selectedRows(3).value(0).data().toString());
            ui->lineEdit_phone_m_2->setText(select->selectedRows(4).value(0).data().toString());
            ui->lineEdit_address_m_2->setText(select->selectedRows(5).value(0).data().toString());


}

void MainWindow::on_modifyEmployeeDB_clicked()
{
    QString cin = ui->lineEdit->text();
    QString fullname= ui->lineEdit_fullname_m_2->text();
    int age= ui->lineEdit_age_m_2->text().toInt();
    QString email= ui->lineEdit_email_m_2->text();
    int phone= ui->lineEdit_phone_m_2->text().toInt();
    QString address= ui->lineEdit_address_m_2->text();

    int x=0;

    string str_phone = to_string(phone);
    string m = email.toStdString();

    if (fullname=="")
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Name can't be empty"), QMessageBox::Ok);
              x++;
    }

    else if (age<20 || age>26)
    {
        QMessageBox::critical(nullptr, QObject::tr("WARNING"),
                    QObject::tr("Age must be between 20 and 26"), QMessageBox::Ok);
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
    ui->lineEdit->clear();
    ui->lineEdit_fullname_m_2->clear();
    ui->lineEdit_age_m_2->clear();
    ui->lineEdit_email_m_2->clear();
    ui->lineEdit_phone_m_2->clear();
    ui->lineEdit_address_m_2->clear();
    ui->stackedWidget->setCurrentIndex(9);
    ui->empTable->setModel(tableEmployee.show());
    }
}

//ADVANCED FUNCTIONS

//search

void MainWindow::on_searchEmployee_clicked()
{
    QString fullname=ui->lineEdit_6->text();
   if(fullname=="")
   {
       employee().show_notification("Search","search field is empty");
   }
   else
   {
       ui->empTable->setModel(tableEmployee.search(fullname));
       ui->lineEdit_fullname_m_2->clear();
       ui->lineEdit_age_m_2->clear();
       ui->lineEdit_email_m_2->clear();
       ui->lineEdit_phone_m_2->clear();
       ui->lineEdit_address_m_2->clear();
      int n= ui->empTable->model()->rowCount();
      if(n==0)
      {
          employee().show_notification("Search","employee not found \n verify the name");
      }
   }
}

//sort Desc

void MainWindow::on_ageDESC_clicked()
{
    ui->empTable->setModel(tableEmployee.show_Desc());
}

//sort Asc

void MainWindow::on_ageASC_clicked()
{
    ui->empTable->setModel(tableEmployee.show_Asc());
}

//Pdf creation

void MainWindow::pdf()
{
    QString strStream;
    QString currentDate = QDateTime().currentDateTime().toString();
    QTextStream out(&strStream);
    const int rowCount = ui->empTable->model()->rowCount();
    const int columnCount = ui->empTable->model()->columnCount();
    out <<
     "<html>\n"
    "<head>\n"
    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
    <<QString("<title>%1</title>\n").arg("strTitle")
    <<"</head>\n"
    "<body bgcolor=#ffffff link=#5000A0>\n"
     <<QString(currentDate)
    <<//"<align='right'> " << datefich << "</align>"
    "<center> <img src="":/img/img/logo.png"" width=""100"" height=""100"" > <br> <br><H1>LIST OF EMPLOYEES</H1> <br> <br><table border=1 cellspacing=0 cellpadding=2>\n";
    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    if (!ui->empTable->isColumnHidden(column))
    out << QString("<th>%1</th>").arg(ui->empTable->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";
    // data table
    for (int row = 0; row < rowCount; row++)
    {
    out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
    for (int column = 0; column < columnCount; column++)
    {
    if (!ui->empTable->isColumnHidden(column))
    {
    QString data = ui->empTable->model()->data(ui->empTable->model()->index(row, column)).toString().simplified();
    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
    }
    }
    out << "</tr>\n";
    }
    out <<  "</table> </center>\n"
         "<br> <br> <br> <br>"
    "</body>\n"
    "<footer>\n"
            "<div class = ""container"">"
                "<div class = ""row"">"
                    "<div>"
                        "<div><img src="":/img/img/icons8-facebook-30.png""> <span>Bon App TN </div>\n"
                        "<br>"
                        "<div><img src="":/img/img/icons8-instagram-30.png""> <span>@bonApp.tn </div>\n"
                        "<p>Generated from : Bon App.exe "
                    "</div>"
                "</div>"
            "</div>"
    "</footer>\n"
    "</html>\n";
    QString filter = "pdf (*.pdf) ";
    QString fileName = QFileDialog::getSaveFileName(this, "save in", QDir::homePath(),filter);
    if(fileName.isEmpty()&&fileName.isNull())
    {
        employee().show_notification("error","exporting is cancelled");
    }
    else
    {
    QPrinter printer (QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);
    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
    employee().show_notification("PDF creation","File created successfully");
    }
}

void MainWindow::on_getEmployeePdf_clicked()
{
    if(ui->empTable->verticalHeader()->count()==0)
        {
            employee().show_notification("error","no data to export");
        }
        else
        {
            pdf();
        }
}

//stats

void MainWindow::on_toolButton_pdf_clicked()
{
    ui->stackedWidget->setCurrentIndex(34);
    // set dark background gradient:
    QColor color = QColor(251, 255, 225);
    ui->customPlot->setBackground(QBrush(color));

    // create empty bar chart objects:
    QCPBars *age_1 = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    age_1->setAntialiased(true);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    labels << "20" << "21" << "22" << "23" << "24" << "25" << "26";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTicks(false);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->setRange(0, 8);
    ui->customPlot->xAxis->setLabel("AGES");
    ui->customPlot->xAxis->setBasePen(QPen(Qt::black));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::black));
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->customPlot->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot->xAxis->setLabelColor(Qt::black);

    // prepare y axis:
    ui->customPlot->yAxis->setRange(0, 5);
    ui->customPlot->yAxis->setPadding(1); // a bit more space to the left border
    ui->customPlot->yAxis->setLabel("NUMBER OF EMPLOYEES");
    ui->customPlot->yAxis->setBasePen(QPen(Qt::black));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::black));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::black));
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setLabelColor(Qt::black);
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> age1;
    int stat1, stat2,stat3,stat4,stat5,stat6,stat7;
    employee().getStat_1(stat1);
    employee().getStat_2(stat2);
    employee().getStat_3(stat3);
    employee().getStat_4(stat4);
    employee().getStat_5(stat5);
    employee().getStat_6(stat6);
    employee().getStat_7(stat7);
    age1 << stat1 << stat2 << stat3 << stat4 << stat5 << stat6 << stat7;
    age_1->setData(ticks, age1);
    // setup legend:
    ui->customPlot->legend->setVisible(false);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->setInteractions(QCP::iRangeDrag);
    foreach(QCPAxisRect *rect, ui->customPlot->axisRects())
         rect->setRangeDrag(Qt::Horizontal);
}

//END EMPLOYEE

//START SHIFTS

//FUNCTIONS

//show employee id + add shifts

void MainWindow::on_addShiftsInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(6+25);
    ui->emptable_2->setModel(tableEmployee1.show_shifts());
}

void MainWindow::on_emptable_2_clicked(const QModelIndex &index)
{
    QString val=ui->emptable_2->model()->data(index).toString();
    ui->lineEdit_cin_shifts->setText(val);

}

void MainWindow::on_addShiftsToDB_clicked()
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
        ui->stackedWidget->setCurrentIndex(5+25);
    }
}

//show employee + shifts

void MainWindow::on_viewAllShifts_clicked()
{
    ui->stackedWidget->setCurrentIndex(7+25);
    ui->shiftsTab->setModel(tableShifts.show());
}

//delete shifts

void MainWindow::on_shiftsTab_clicked(const QModelIndex &index)
{
    QString val=ui->shiftsTab->model()->data(index).toString();
    ui->lineEdit_toDo->setText(val);
}

void MainWindow::on_deleteShiftsFromDB_clicked()
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
            ui->stackedWidget->setCurrentIndex(8+25);
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
            ui->stackedWidget->setCurrentIndex(5+25);
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
    if(ui->shiftsTab->verticalHeader()->count()==0)
    {
        employee().show_notification("error","No valid Data");
    }
    else
    {
        ui->shiftsTab->setModel(tableShifts.show_Asc());
    }
}

//sort Desc

void MainWindow::on_toolButton_delete_7_clicked()
{
    if(ui->shiftsTab->verticalHeader()->count()==0)
    {
        employee().show_notification("error","No valid Data");
    }
    else
    {
        ui->shiftsTab->setModel(tableShifts.show_Desc());
    }
}

//END SHIFTS


void MainWindow::on_getShiftsStat_clicked()
{
    shifts().stats(ui->shiftsStat);
    ui->stackedWidget->setCurrentIndex(10+25);
}

void MainWindow::on_viewRoles_clicked()
{
    Role.stats(ui->rolestats);
    ui->stackedWidget->setCurrentIndex(11+25);
    Role.getTable(ui->roleTable);
}

void MainWindow::on_addRolePage_clicked()
{
    QSqlQueryModel *mod= new QSqlQueryModel();
        mod->setQuery(("select FULLNAME from EMPLOYEE"));
        ui->nameRole->setModel(mod);
        ui->modifyRole->setDisabled(true);
        ui->nameRole_Edit->setDisabled(true);
        ui->cinrole_Edit->setDisabled(true);
        ui->role_Edit->setDisabled(true);
        ui->earningrole_Edit->setDisabled(true);
        ui->bonusrole_Edit->setDisabled(true);
        ui->addingLabel->setText("Add Role");
        ui->modificationLabel->setText("Modification Unavailable");
        ui->stackedWidget->setCurrentIndex(12+25);
}

void MainWindow::on_nameRole_activated(const QString &arg1)
{
            QSqlQuery query;
            query.prepare("SELECT CIN FROM EMPLOYEE WHERE FULLNAME= '"+arg1+"'");
            if(query.exec())
            {
                while (query.next())
                {
                    ui->cinrole->setText(query.value(0).toString());
                }
            }
}

void MainWindow::on_addRoleToDB_clicked()
{
    QString cin= ui->cinrole->text();
    QString name= ui->nameRole->currentText();
    QString empRole= ui->role->text();
    int earning= ui->earningrole->text().toInt();
    int bonus= ui->bonusrole->text().toInt();

    role e(cin,name,empRole,earning,bonus);
    bool toTest =e.add();
    if(toTest)
    {
        employee().show_notification("Add Role","role added successfully");
    }
    ui->nameRole->clear();
    ui->earningrole->clear();
    ui->bonusrole->clear();
    ui->cinrole->clear();
    ui->role->clear();
    Role.getTable(ui->roleTable);
    ui->stackedWidget->setCurrentIndex(11+25);
}

void MainWindow::on_previousRolePage_clicked()
{
    Role.prevPage(ui->roleTable);
}

void MainWindow::on_nextRolePage_clicked()
{
    Role.nextPage(ui->roleTable);
}

void MainWindow::on_deleteRole_clicked()
{
    QString var = ui->toDelete->text();
    Role.remove(var,ui->roleTable);
    ui->toDelete->clear();
}

void MainWindow::on_roleTable_cellClicked(int row)
{
    QString var=ui->roleTable->item(row,0)->text();
    ui->toDelete->setText(var);
}

void MainWindow::on_editRole_clicked()
{
    foreach(QModelIndex index,ui->roleTable->selectionModel()->selectedIndexes())
    {
            int row=index.row();

    if (row >=0)
    {
        QString cin=ui->roleTable->item(row,0)->text();
        QString name=ui->roleTable->item(row,1)->text();
        QString roleEdit=ui->roleTable->item(row,2)->text();
        QString earning=ui->roleTable->item(row,3)->text();
        QString bonus=ui->roleTable->item(row,4)->text();
            ui->nameRole_Edit->setText(name);
            ui->cinrole_Edit->setText(cin);
            ui->role_Edit->setText(roleEdit);
            ui->earningrole_Edit->setText(earning);
            ui->bonusrole_Edit->setText(bonus);
            ui->addRoleToDB->setDisabled(true);
            ui->nameRole->setDisabled(true);
            ui->cinrole->setDisabled(true);
            ui->role->setDisabled(true);
            ui->earningrole->setDisabled(true);
            ui->bonusrole->setDisabled(true);
            ui->modifyRole->setDisabled(false);
            ui->nameRole_Edit->setDisabled(false);
            ui->cinrole_Edit->setDisabled(false);
            ui->role_Edit->setDisabled(false);
            ui->earningrole_Edit->setDisabled(false);
            ui->bonusrole_Edit->setDisabled(false);
            ui->addingLabel->setText("Adding Role Disabled");
            ui->modificationLabel->setText("Modify Role");
            ui->stackedWidget->setCurrentIndex(12+25);
    }
    else
    {
        qDebug("no item selected");
    }
    }
}

void MainWindow::on_modifyRole_clicked()
{
    QString cin= ui->cinrole_Edit->text();
    QString name= ui->nameRole_Edit->text();
    QString empRole= ui->role_Edit->text();
    int earning= ui->earningrole_Edit->text().toInt();
    int bonus= ui->bonusrole_Edit->text().toInt();

    role e(cin,name,empRole,earning,bonus);
    bool toTest =e.modify();
    if(toTest)
    {
        employee().show_notification("Modify Role","role modified successfully");
    }
    ui->nameRole_Edit->clear();
    ui->earningrole_Edit->clear();
    ui->bonusrole_Edit->clear();
    ui->cinrole_Edit->clear();
    ui->role_Edit->clear();
    ui->addRoleToDB->setDisabled(false);
    ui->nameRole->setDisabled(false);
    ui->cinrole->setDisabled(false);
    ui->role->setDisabled(false);
    ui->earningrole->setDisabled(false);
    ui->bonusrole->setDisabled(false);
    ui->modifyRole->setDisabled(true);
    ui->nameRole_Edit->setDisabled(true);
    ui->cinrole_Edit->setDisabled(true);
    ui->role_Edit->setDisabled(true);
    ui->earningrole_Edit->setDisabled(true);
    ui->bonusrole_Edit->setDisabled(true);
    Role.getTable(ui->roleTable);
    ui->stackedWidget->setCurrentIndex(11+25);
}

void MainWindow::on_searchRoleInDB_clicked()
{
    QString toSearch = ui->searchRole->text();
    Role.search(toSearch,ui->roleTable);
}

void MainWindow::on_sortRole_clicked()
{
    Role.sortRole(ui->roleTable);
}

void MainWindow::on_sortEarning_clicked()
{
    Role.sortEarning(ui->roleTable);
}

void MainWindow::on_backToRoleList_clicked()
{
    ui->stackedWidget->setCurrentIndex(7+25);
}

void MainWindow::on_backToRolefct_clicked()
{
    ui->stackedWidget->setCurrentIndex(1+6);
}

void MainWindow::on_backToRolefct_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(11+25);
}

void MainWindow::on_backToRolesFct_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(11+25);
}

void MainWindow::on_roleStats_clicked()
{
    Role.stats(ui->rolestats);
    ui->stackedWidget->setCurrentIndex(13+25);
}


//ahmed functions


void MainWindow::on_recettesBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(39);
    QApplication::beep();
}

void MainWindow::on_queueBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(40);
    QApplication::beep();
}




void MainWindow::slotDeleteRow()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){

        if (QMessageBox::warning(this,
                                 tr("Supprimer ligne"),
                                 tr("Etes vous sur de bien vouloir supprimer cette ligne définitivement ??"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            qDebug() << "Deleting row " << row;
            tableModel->removeRows(row, 1);
            tableModel->submitAll();
            return;
        }
    }
}

void MainWindow::slotAddBefore()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){
        tableModel->insertRows(row, 1);
    }
}


void MainWindow::slotAddEnd()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){
        tableModel->insertRows(tableModel->rowCount(), 1);
    }
}


void MainWindow::slotAddAfter()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){
        tableModel->insertRows(row+1, 1);
    }
}



void MainWindow::on_rafficherTable_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "Adding context menu";
    QMenu * menu = new QMenu(this);

    QAction * addBefore = new QAction(tr("Ajouter avant"), this);
    QAction * addAfter = new QAction(tr("Ajouter apres"), this);
    QAction * addEnd = new QAction(tr("Ajouter à la fin"), this);
    QAction * deleteRow = new QAction(tr("Supprimer"), this);

    connect(addBefore, SIGNAL(triggered()), this, SLOT(slotAddBefore()));
    connect(addAfter, SIGNAL(triggered()), this, SLOT(slotAddAfter()));
    connect(addEnd, SIGNAL(triggered()), this, SLOT(slotAddEnd()));
    connect(deleteRow, SIGNAL(triggered()), this, SLOT(slotDeleteRow()));

    menu->addAction(addBefore);
    menu->addAction(addAfter);
    menu->addAction(addEnd);
    menu->addAction(deleteRow);

    qDebug() << "done context menu";

    menu->popup(ui->rafficherTable->viewport()->mapToGlobal(pos));
}

void MainWindow::on_lineEdit111_textChanged(const QString &arg1)
{
    tableModel->setFilter("lower(NOM) LIKE lower('%" + arg1 + "%')");
}


void MainWindow::on_toolButton111_clicked()
{
    ui->lineEdit111->setText("");
}

void MainWindow::on_pushButtonpdf_clicked()
{
    QDialog QFileDialog;
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    /*QPrinter printer;*/





    QTextDocument doc;
    QSqlQuery q;
    q.prepare("SELECT * FROM FILEATTENTE");
    q.exec();
    QString pdf="<br> <img src='C:\\Users\\Ahmed2\\Documents\\GitHub\\BonApp\\logo.png' height='42' width='144'/> <b><h1  style='color:blue'>File d'attente :  </b><br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> RECETTE </th> <th> ETAT </th>  </tr>" ;

    while ( q.next()) {
        pdf = pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>   <td>   " + q.value(1).toString() +"</td>   <td>" +q.value(2).toString() +"  "" " "</td>";
    }

    doc.setHtml(pdf);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
    //ui->rafficherTable->render(&printer);
}

void MainWindow::on_pushButtonrec1_clicked()
{
    queue nqueue = queue(0, ui->lineEditrec1->text(), ui->lineEditrec2->text());

    nqueue.ajouter();

    ui->queueTableView->setModel(Qtmp.afficher());

}

void MainWindow::on_pushButtonrec3_clicked()
{
    if(ui->queueTableView->selectionModel()->hasSelection())
    {


            QModelIndexList selection = ui->queueTableView->selectionModel()->selectedRows();
            qDebug() << "selection count : " << selection.count();


            for(int i=0; i< selection.count(); i++)
            {
                if(i==0)
                {
                    if (QMessageBox::warning(this,
                                             tr("Supprimer lignes"),
                                             tr("Etes vous sur de bien vouloir supprimer les lignes selectionnés définitivement ?"),
                                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
                    {
                        break;
                    }
                }
                QModelIndex index = selection.at(i);
                int id = ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),0)).toString().toInt();
                qDebug() << "deleting queue id : " << id;
                Qtmp.supprimer(id);
            }


    }

    ui->queueTableView->setModel(Qtmp.afficher());

}

void MainWindow::on_pushButtonrec4_clicked()
{
    if(ui->queueTableView->selectionModel()->hasSelection())
    {

        if (QMessageBox::warning(this,
                                 tr("Supprimer toutes les taches"),
                                 tr("Etes vous sur de bien vouloir supprimer toutes les lignes définitivement ?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            Qtmp.supprimer(-1);
        }

    }

    ui->queueTableView->setModel(Qtmp.afficher());
}

void MainWindow::on_queueTableView_pressed(const QModelIndex &index)
{

}

void MainWindow::on_queueTableView_clicked(const QModelIndex &index)
{
    lastQueue = ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),0)).toString().toInt();
    ui->lineEditrec3->setText(ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),1)).toString());
    ui->lineEditrec4->setText(ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),2)).toString());
    ui->pushButtonrec2->setEnabled(true);
}

void MainWindow::on_pushButtonrec2_clicked()
{
    queue nqueue = queue(lastQueue, ui->lineEditrec3->text(), ui->lineEditrec4->text());

    nqueue.modifier();

    ui->queueTableView->setModel(Qtmp.afficher());
}

void MainWindow::on_returnbtn1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_returnbtn1_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButtonrec3_pressed()
{

}







//ahmed functions end









//karim functions start


void MainWindow::on_pushbuttonmat_clicked()
{
  ui->stackedWidget ->setCurrentIndex(43);
}

void MainWindow::on_pushbuttonprod_clicked()
{
    ui->stackedWidget ->setCurrentIndex(41);
    QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
}

void MainWindow::on_pushbuttonback2_clicked()
{
      ui->stackedWidget ->setCurrentIndex(0);
}

void MainWindow::on_pushbuttonback1_clicked()
{
    ui->stackedWidget ->setCurrentIndex(0);
}

void MainWindow::on_pushButtonaj1_clicked()
{
     ui->stackedWidget ->setCurrentIndex(42);
      QSound::play(":./Desktop/QT/ffqf/crunch.wav");
}

void MainWindow::on_pushButtonback_clicked()
{
    ui->stackedWidget ->setCurrentIndex(41);
}

void MainWindow::on_pushButtonback3_clicked()
{
   ui->stackedWidget ->setCurrentIndex(43);
}

void MainWindow::on_pushButtonaj2_clicked()
{
   ui->stackedWidget ->setCurrentIndex(44);
}

void MainWindow::on_pushButtonval2_clicked()
{
  int id_materiel=ui->lineEditmat1 ->text().toInt();
int nb_materiel=ui->lineEditmat4 ->text().toInt();
QString nom_materiel=ui->lineEditmat2 ->text();
QString date_entr=ui->lineEditmat3 ->text();
materiel M( nb_materiel,id_materiel,nom_materiel,date_entr);
bool test=M.ajouter();
        if(test)
{
            ui->tableView22->setModel(Mtmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));
            QMessageBox::information(nullptr,QObject::tr("ok"),
                           QObject::tr("ajout effectué\n"
                                       "click Cancel to exit"), QMessageBox::Cancel);

        }
        else
            QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("Ajout non effectué\n"
                                                                            "Click Cancel to exit."), QMessageBox::Cancel);
}


void MainWindow::on_pushButtonsup2_clicked()
{
    int id_materiel =ui->lineEdit_suppmat ->text().toInt();
    bool test=Mtmp.supprimer(id_materiel);
    if(test)
    {
         ui->tableView22->setModel(Mtmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));
      QMessageBox::information(nullptr,QObject::tr("OK"),
              QObject::tr("Suppression effectué.\n"
                          "click Cancel to exit"), QMessageBox::Cancel);
    }
    QMessageBox::information(nullptr,QObject::tr("NOT OK"),
            QObject::tr("Suppression non effectué.\n"
                        "click Cancel to exit"),QMessageBox::Cancel);

}



void MainWindow::on_pushButtonvalid1_clicked()
{
    int id_ingrediant=ui->lineEditprod1 ->text().toInt();
  int quantite_ingredient=ui->lineEditprod4 ->text().toInt();
  QString nom_ingredient=ui->lineEditprod2 ->text();
  QString date_exp=ui->lineEditprod3 ->text();
  produit P( quantite_ingredient,id_ingrediant,nom_ingredient,date_exp);
  bool test=P.ajouter();
          if(test)
  {      ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));

              QMessageBox::information(nullptr,QObject::tr("ok"),
                             QObject::tr("ajout effectué\n"
                                         "click Cancel to exit"), QMessageBox::Cancel);

          }
          else
              QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("Ajout non effectué\n"
                                                                              "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButtonsup1_clicked()
{
    //ui->stackedWidget ->setCurrentIndex(0);
    int id_produit =ui->lineEditsup1 ->text().toInt();
    bool test=Ptmp.supprimer(id_produit);
    if(test)
    {
         ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
     N.notifications_supprimerproduit();
    }
    QMessageBox::information(nullptr,QObject::tr("NOT OK"),
            QObject::tr("Suppression non effectué.\n"
                        "click Cancel to exit"),QMessageBox::Cancel);

}




void MainWindow::on_pushButtonmod1_clicked()
{
    if (ui->pushButtonmod1->isChecked())
               {
                  // ui->pushButton_modifier->setDisabled(true);
                  // ui->pushButton_6->setText("Terminer Modif");
                   QSqlTableModel *tableModel= new QSqlTableModel();
                   tableModel->setTable("PRODUIT");
tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

           //tableModel->setFilter("where lower(NOM_INGREDIENT) like lower('%" + ui->lineEdit_13->text() + "%')");
                   tableModel->select();
                   ui->tableView11->setModel(tableModel);
               }
               else
               {
                   //ui->pushButton_6->setText("Modifier");
                   ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));

               }
}

QString MainWindow::getSearchText()
{
    return ui->lineEditrech1->text();
}

void MainWindow::on_lineEditrech1_textChanged(const QString &arg1)
{
    ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
}

void MainWindow::on_comboboxtri1_currentIndexChanged(int index)
{
    ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
}



void MainWindow::on_PDF11_clicked()
{
     QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
            if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append("liste des produits.pdf"); }

            QPrinter printer(QPrinter::PrinterResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPaperSize(QPrinter::A4);
            printer.setOutputFileName(fileName);

            QTextDocument doc;
            QSqlQuery q;
            q.prepare("SELECT * FROM PRODUIT ");
            q.exec();
            QString pdf="<br> <img src='C:\\Users\\Karim\\Desktop\\QT\\BonApp\\img\\logo.png' height='42' width='144'/> <h1  style='color:#d88855'>       LISTE DES PRODUITS  <br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> nom </th> <th> date_exp </th>  <th> quantité  </th>   </tr>" ;


            while ( q.next()) {

                pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>  <td>   " + q.value(1).toString() +"</td>  <td>" +q.value(2).toString() +"  "" " "</td>      <td>     "+q.value(3).toString()+"--"+"</td>       <td>"+q.value(4).toString()+"             </td>" ;

            }
            doc.setHtml(pdf);
            doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
            doc.print(&printer);

}

void MainWindow::on_stat11_clicked()
{
    sec = new SecDialog(this);
    sec->show();
}
void MainWindow::on_tableView11_activated(const QModelIndex &index)
{
    int totest = ui->tableView11->model()->data(index).toInt();
    QString s=QString::number(totest);
    ui->lineEditsup1->setText(s);
}

void MainWindow::on_pushButtonmod2_clicked()
{
    if (ui->pushButtonmod2->isChecked())
               {
                  // ui->pushButton_modifier->setDisabled(true);
                  // ui->pushButton_10->setText("Terminer Modif");
                   QSqlTableModel *tableModel= new QSqlTableModel();
                   tableModel->setTable("MATERIEL");
tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

           //tableModel->setFilter("where lower(NOM_INGREDIENT) like lower('%" + ui->lineEdit_13->text() + "%')");
                   tableModel->select();
                   ui->tableView22->setModel(tableModel);
               }
               else
               {
                   //ui->pushButton_10->setText("Modifier");
                   ui->tableView22->setModel(Mtmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));

               }
}

void MainWindow::on_PDF_mat_clicked()
{
    QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
   QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
           if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append("liste des materiels.pdf"); }

           QPrinter printer(QPrinter::PrinterResolution);
           printer.setOutputFormat(QPrinter::PdfFormat);
           printer.setPaperSize(QPrinter::A4);
           printer.setOutputFileName(fileName);

           QTextDocument doc;
           QSqlQuery q;
           q.prepare("SELECT * FROM MATERIEL ");
           q.exec();
           QString pdf="<br> <img src='C:\\Users\\Karim\\Desktop\\QT\\BonApp\\img\\logo.png' height='42' width='144'/> <h1  style='color:#d88855'>       LISTE DES PRODUITS  <br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> nom </th> <th> date_exp </th>  <th> quantité  </th>   </tr>" ;


           while ( q.next()) {

               pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>  <td>   " + q.value(1).toString() +"</td>  <td>" +q.value(2).toString() +"  "" " "</td>      <td>     "+q.value(3).toString()+"--"+"</td>       <td>"+q.value(4).toString()+"             </td>" ;

           }
           doc.setHtml(pdf);
           doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
           doc.print(&printer);

}

void MainWindow::on_pushButton_mat_clicked()
{
    sec = new SecDialog(this);
    sec->show();
}



void MainWindow::on_lineEdit_rech_mat_textChanged(const QString &arg1)
{
     ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri2->currentIndex()));
}

void MainWindow::on_comboboxtri2_currentIndexChanged(int index)
{
    ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri2->currentIndex()));
}

//karim funcs end











void MainWindow::on_pushButton_13_clicked()
{
    ui->stackedWidget ->setCurrentIndex(0);
}
