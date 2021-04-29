#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "shifts.h"
#include "role.h"
#include "QCustomPlot.h"
#include <QSound>
#include <QTime>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QTimeEdit>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include <QObject>
#include <QString>
#include<string>
#include<QDate>
#include <algorithm>
#include <vector>
#include<QSqlRecord>
#include<QSqlQuery>
#include <QPixmap>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QTextDocument>
#include <QDateTime>
#include <QMediaPlayer>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
        QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//INDEXING



void MainWindow::on_back_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

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
    ui->stackedWidget->setCurrentIndex(1);
    }
}

//delete employee (to change)

void MainWindow::on_empTable_clicked(const QModelIndex &index)
{
    QString val = ui->empTable->model()->data(index).toString();
    ui->lineEdit->setText(val);
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

void MainWindow::on_empTable_activated()
{
            QItemSelectionModel *select = ui->empTable->selectionModel();
            ui->lineEdit_fullname_m_2->setText(select->selectedRows(1).value(0).data().toString());
            ui->lineEdit_age_m_2->setText(select->selectedRows(2).value(0).data().toString());
            ui->lineEdit_email_m_2->setText(select->selectedRows(3).value(0).data().toString());
            ui->lineEdit_phone_m_2->setText(select->selectedRows(4).value(0).data().toString());
            ui->lineEdit_address_m_2->setText(select->selectedRows(5).value(0).data().toString());


}

void MainWindow::on_toolButton_modify_clicked()
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
    ui->stackedWidget->setCurrentIndex(3);
    ui->empTable->setModel(tableEmployee.show());
    }
}

//ADVANCED FUNCTIONS

//search

void MainWindow::on_toolButton_11_clicked()
{
    QString fullname=ui->lineEdit->text();
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

void MainWindow::on_toolButton_12_clicked()
{
    ui->empTable->setModel(tableEmployee.show_Desc());
}

//sort Asc

void MainWindow::on_toolButton_13_clicked()
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

void MainWindow::on_toolButton_modify_2_clicked()
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
    ui->stackedWidget->setCurrentIndex(9);
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

void MainWindow::on_toolButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->emptable_2->setModel(tableEmployee1.show_shifts());
}

void MainWindow::on_emptable_2_clicked(const QModelIndex &index)
{
    QString val=ui->emptable_2->model()->data(index).toString();
    ui->lineEdit_cin_shifts->setText(val);

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
    ui->lineEdit_toDo->setText(val);
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


void MainWindow::on_toolButton_delete_8_clicked()
{
    shifts().stats(ui->shiftsStat);
    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_toolButton_8_clicked()
{
    Role.stats(ui->rolestats);
    ui->stackedWidget->setCurrentIndex(11);
    Role.getTable(ui->roleTable);
}

void MainWindow::on_gotoRole_clicked()
{
    QSqlQueryModel *mod= new QSqlQueryModel();
        mod->setQuery(("select FULLNAME from EMPLOYEE"));
        ui->nameRole->setModel(mod);        
        ui->modifyRoleButton->setDisabled(true);
        ui->nameRole_Edit->setDisabled(true);
        ui->cinrole_Edit->setDisabled(true);
        ui->role_Edit->setDisabled(true);
        ui->earningrole_Edit->setDisabled(true);
        ui->bonusrole_Edit->setDisabled(true);
        ui->addingLabel->setText("Add Role");
        ui->modificationLabel->setText("Modification Unavailable");
        ui->stackedWidget->setCurrentIndex(12);
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

void MainWindow::on_addRoleButton_clicked()
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
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_previousPage_clicked()
{
    Role.prevPage(ui->roleTable);
}

void MainWindow::on_nextPage_clicked()
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
            ui->addRoleButton->setDisabled(true);
            ui->nameRole->setDisabled(true);
            ui->cinrole->setDisabled(true);
            ui->role->setDisabled(true);
            ui->earningrole->setDisabled(true);
            ui->bonusrole->setDisabled(true);
            ui->modifyRoleButton->setDisabled(false);
            ui->nameRole_Edit->setDisabled(false);
            ui->cinrole_Edit->setDisabled(false);
            ui->role_Edit->setDisabled(false);
            ui->earningrole_Edit->setDisabled(false);
            ui->bonusrole_Edit->setDisabled(false);
            ui->addingLabel->setText("Adding Role Disabled");
            ui->modificationLabel->setText("Modify Role");
            ui->stackedWidget->setCurrentIndex(12);
    }
    else
    {
        qDebug("no item selected");
    }
    }
}

void MainWindow::on_modifyRoleButton_clicked()
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
    ui->addRoleButton->setDisabled(false);
    ui->nameRole->setDisabled(false);
    ui->cinrole->setDisabled(false);
    ui->role->setDisabled(false);
    ui->earningrole->setDisabled(false);
    ui->bonusrole->setDisabled(false);
    ui->modifyRoleButton->setDisabled(true);
    ui->nameRole_Edit->setDisabled(true);
    ui->cinrole_Edit->setDisabled(true);
    ui->role_Edit->setDisabled(true);
    ui->earningrole_Edit->setDisabled(true);
    ui->bonusrole_Edit->setDisabled(true);
    Role.getTable(ui->roleTable);
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_search_clicked()
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

void MainWindow::on_back_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_back_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_back_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::on_stats_clicked()
{
    Role.stats(ui->rolestats);
    ui->stackedWidget->setCurrentIndex(13);
}
