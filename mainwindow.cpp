#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include"gateauxc.h"
#include"commandec.h"
#include"category.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include<string>
#include <algorithm>
#include <vector>
#include<QDir>
#include<QFileDialog>
#include<QChart>
#include<QSound>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
     //le slot update_label suite à la reception du signal readyRead (reception des données).
}

void MainWindow::update_label()
{
    data=A.read_from_arduino();
    if(data.isNull())return;
    qDebug()<<data;
    if(data=="1")

        ui->label_3->setText("ON"); // si les données reçues de arduino via la liaison série sont égales à 1
    // alors afficher ON

    else if (data=="0")

        ui->label_3->setText("OFF");   // si les données reçues de arduino via la liaison série sont égales à 0
     //alors afficher ON
}

MainWindow::~MainWindow()
{
    delete ui;
}
QSound audio (":/img/img/click.wav");
void MainWindow::on_goToOrders_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    audio.play();
}
void MainWindow::on_goToProducts_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    audio.play();
}

void MainWindow::on_goToStock_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_goToClient_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
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
    QString type=ui->type->text();
    ui->stackedWidget->setCurrentIndex(3);
    ui->lineEdit_type->setText(type);
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

void MainWindow::on_addProduct_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_showProducts_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

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

