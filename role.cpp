#include "role.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QSqlRecord>
#include <QAbstractItemModel>
#include <map>
#include <QMessageBox>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>

using namespace std;

int role::currentpage = 0;
int role::maxPerPage = 2;

role::role()
{
    cin="";
    name="";
    roles="";
    earning=0;
    bonus=0;
}

role::role(QString cin, QString name, QString roles, int earning, int bonus)
{
    this->cin=cin;
    this->name=name;
    this->roles=roles;
    this->earning=earning;
    this->bonus=bonus;
}

QString role::get_cin(){return cin;}

QString role::get_name(){return name;}

QString role::get_role(){return roles;}

int role::get_earning(){return earning;}

int role::get_bonus(){return bonus;}

//crud functions

bool role::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO ROLE (CIN, NAME, ROLE, EARNING, BONUS)"
                  "VALUES(:CIN, :NAME, :ROLE, :EARNING, :BONUS)");
    query.bindValue(":CIN",cin);
    query.bindValue(":NAME",name);
    query.bindValue(":ROLE",roles);
    query.bindValue(":EARNING",earning);
    query.bindValue(":BONUS",bonus);
    return query.exec();
}

void role::remove(QString toSearch,QTableWidget *roleTable)
{
    QSqlQuery query;
    query.prepare("DELETE FROM ROLE WHERE CIN = :CIN");
    query.bindValue(":CIN",toSearch);
    query.exec();
    role::getTable(roleTable);
}

bool role::modify()
{
    QSqlQuery query;
    query.prepare("UPDATE ROLE SET CIN= :CIN, NAME= :NAME, ROLE= :ROLE, EARNING= :EARNING, BONUS= :BONUS WHERE CIN= :CIN");
    query.bindValue(":CIN",cin);
    query.bindValue(":NAME",name);
    query.bindValue(":ROLE",roles);
    query.bindValue(":EARNING",earning);
    query.bindValue(":BONUS",bonus);
    return query.exec();
}

void role::showPage(QTableWidget* roleTable)
{
    for(int i=0; i<roleTable->rowCount(); i++)
        {
            roleTable->showRow(i);
            if(i > role::currentpage*role::maxPerPage + (role::maxPerPage-1) || i < role::currentpage*role::maxPerPage)
                 roleTable->hideRow(i);
        }
}

void role::nextPage(QTableWidget *roleTable)
{
    role::currentpage++;
        if(role::currentpage > roleTable->rowCount()/role::maxPerPage)role::currentpage = roleTable->rowCount()/role::maxPerPage;
        role::showPage(roleTable);
}

void role::prevPage(QTableWidget *roleTable)
{
    role::currentpage--;
    if(role::currentpage < 0)role::currentpage = 0;
        role::showPage(roleTable);
}

void role::getTable(QTableWidget *roleTable)
{
    QSqlQuery query("SELECT * FROM ROLE");
        roleTable->clear();

        int numberOfRows = 0;
        if(query.last())
        {
            numberOfRows =  query.at() + 1;
            query.first();
            query.previous();
        }
        roleTable->setRowCount(numberOfRows);
        int columns = query.record().count();
        QSqlRecord record = query.record();
        roleTable->setColumnCount(columns);
        QStringList columnsNames = QStringList();
        for(int s=0;s<columns;s++)
        {
            columnsNames<<record.fieldName(s);
        }
        roleTable->setHorizontalHeaderLabels(columnsNames);
        int row = 0;
        while (query.next()) {
            for(int column = 0; column < columns; column++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(query.value(column).toString());
                if(column == 0)
                {
                    auto flags = item->flags();
                    flags.setFlag(Qt::ItemIsEditable, false);
                    item->setFlags(flags);
                }
                roleTable->setItem(row, column, item);
            }
            row++;
         }
        role::currentpage = 0;
        role::showPage(roleTable);
}

void role::search(QString toSearch, QTableWidget *roleTable)
{
    QSqlQuery query("SELECT * FROM ROLE WHERE ROLE='"+toSearch+"'");
        roleTable->clear();

        int numberOfRows = 0;
        if(query.last())
        {
            numberOfRows =  query.at() + 1;
            query.first();
            query.previous();
        }
        roleTable->setRowCount(numberOfRows);
        int columns = query.record().count();
        QSqlRecord record = query.record();
        roleTable->setColumnCount(columns);
        QStringList columnsNames = QStringList();
        for(int s=0;s<columns;s++)
        {
            columnsNames<<record.fieldName(s);
        }
        roleTable->setHorizontalHeaderLabels(columnsNames);
        int row = 0;
        while (query.next()) {
            for(int column = 0; column < columns; column++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(query.value(column).toString());
                if(column == 0)
                {
                    auto flags = item->flags();
                    flags.setFlag(Qt::ItemIsEditable, false);
                    item->setFlags(flags);
                }
                roleTable->setItem(row, column, item);
            }
            row++;
         }
}

void role::sortEarning(QTableWidget *roleTable)
{
    QSqlQuery query("SELECT * FROM ROLE ORDER BY EARNING ASC");
        roleTable->clear();

        int numberOfRows = 0;
        if(query.last())
        {
            numberOfRows =  query.at() + 1;
            query.first();
            query.previous();
        }
        roleTable->setRowCount(numberOfRows);
        int columns = query.record().count();
        QSqlRecord record = query.record();
        roleTable->setColumnCount(columns);
        QStringList columnsNames = QStringList();
        for(int s=0;s<columns;s++)
        {
            columnsNames<<record.fieldName(s);
        }
        roleTable->setHorizontalHeaderLabels(columnsNames);
        int row = 0;
        while (query.next()) {
            for(int column = 0; column < columns; column++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(query.value(column).toString());
                if(column == 0)
                {
                    auto flags = item->flags();
                    flags.setFlag(Qt::ItemIsEditable, false);
                    item->setFlags(flags);
                }
                roleTable->setItem(row, column, item);
            }
            row++;
         }
}

void role::sortRole(QTableWidget *roleTable)
{
    QSqlQuery query("SELECT * FROM ROLE ORDER BY ROLE ASC");
        roleTable->clear();

        int numberOfRows = 0;
        if(query.last())
        {
            numberOfRows =  query.at() + 1;
            query.first();
            query.previous();
        }
        roleTable->setRowCount(numberOfRows);
        int columns = query.record().count();
        QSqlRecord record = query.record();
        roleTable->setColumnCount(columns);
        QStringList columnsNames = QStringList();
        for(int s=0;s<columns;s++)
        {
            columnsNames<<record.fieldName(s);
        }
        roleTable->setHorizontalHeaderLabels(columnsNames);
        int row = 0;
        while (query.next()) {
            for(int column = 0; column < columns; column++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(query.value(column).toString());
                if(column == 0)
                {
                    auto flags = item->flags();
                    flags.setFlag(Qt::ItemIsEditable, false);
                    item->setFlags(flags);
                }
                roleTable->setItem(row, column, item);
            }
            row++;
         }
}

QSqlQuery role::getStat_1(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM ROLE WHERE EARNING < 50");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery role::getStat_2(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM ROLE WHERE EARNING > '50' AND EARNING <'100'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery role::getStat_3(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM ROLE WHERE EARNING > '100'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

void role::stats(QFrame *roles)
{
    int stat1,stat2,stat3;

        role::getStat_1(stat1);
        role::getStat_2(stat2);
        role::getStat_3(stat3);
        QPieSeries *series = new QPieSeries();

        series->append("Less than 50Dt per month",stat1);
        series->append("between 50 and 100",stat2);
        series->append("More than 100Dt per month",stat3);

        series->setHoleSize(0.5);
        series->setPieSize(1);

        QPieSlice *less = series->slices().at(0);
        QPieSlice *between = series->slices().at(1);
        QPieSlice *more = series->slices().at(2);

        less->setLabelVisible(true);
        between->setLabelVisible(true);
        more->setLabelVisible(true);

        less->setBrush(Qt::red);
        between->setBrush(Qt::yellow);
        more->setBrush(Qt::blue);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Earnings");
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->setAnimationOptions(QChart::AllAnimations);

        QChartView * chartview = new QChartView(chart);
        chartview->setParent(roles);
}
