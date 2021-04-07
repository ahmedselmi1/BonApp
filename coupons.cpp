#include "coupons.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QTableWidget>
#include <QSqlRecord>
#include <map>
#include <QMessageBox>
#include "QrCode.hpp"
#include <QPainter>
#include <QPrinter>
#include <QTextDocument>
#include "qpainter.h"
#include <QAbstractItemModel>
using std::uint8_t;


using namespace std;

int coupons::currentsorted = -1;
int coupons::currentsorting = -1;
int coupons::currentpage = 0;
int coupons::maxPerPage = 8;
coupons::coupons()
{

}



void coupons::deleteSelectedCoupons(QTableWidget * couponsTable)
{

    map<QString,bool> toDelete;

    foreach(QTableWidgetItem* item,  couponsTable->selectedItems())
    {
        toDelete[couponsTable->item(item->row(),0)->text()] = true;
    }
    map<QString,bool>::iterator it;

    for(it=toDelete.begin(); it!=toDelete.end(); ++it){
            QSqlQuery query;
            query.prepare("DELETE FROM COUPONS WHERE ID = :ID");
            query.bindValue(":ID", it->first);
            query.exec();
          //cout << it->first << " => " << it->second << '\n';
       }
    processCouponTable(couponsTable);
    string aaa = std::to_string(toDelete.size()) + " Coupons deleted successfully";
    QMessageBox::information(nullptr, QObject::tr("Coupons Deleted"),
                      QObject::tr(aaa.c_str()), QMessageBox::Ok);



}


void coupons::printPDF(QTableWidget* couponsTable)
{
    QPixmap pix(couponsTable->size());
      QPainter painter(&pix);
      couponsTable->render(&painter);
      painter.end();
      QPrinter printer(QPrinter::HighResolution);
      printer.setOrientation(QPrinter::Landscape);
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setPaperSize(QPrinter::A4);
      printer.setOutputFileName("coupons.pdf"); // will be in build folder

      painter.begin(&printer);
      double xscale = printer.pageRect().width() / double(pix.width());
      double yscale = printer.pageRect().height() / double(pix.height());
      double scale = qMin(xscale, yscale);
      painter.translate(printer.paperRect().x() + printer.pageRect().width() / 2,
                        printer.paperRect().y() + printer.pageRect().height() / 2);
      painter.scale(scale, scale);
      painter.translate(-couponsTable->width() / 2, -couponsTable->height() / 2);
      painter.drawPixmap(0, 0, pix);

    QTextDocument doc;

    doc.setHtml("");
    doc.drawContents(&painter);

      painter.end();

}
void coupons::processCouponTable(QTableWidget* couponsTable)
{




    QSqlQuery query("SELECT ID, CODE, STARTDATE, ENDDATE, CONSTRAINTS, USES FROM COUPONS");
    couponsTable->clear();


    int numberOfRows = 0;
    if(query.last())
    {
        numberOfRows =  query.at() + 1;
        query.first();
        query.previous();
    }


    couponsTable->setRowCount(numberOfRows);
    int columns = query.record().count();
    QSqlRecord record = query.record();
    couponsTable->setColumnCount(columns);
    QStringList columnsNames = QStringList();
    for(int s=0;s<columns;s++)
    {
        columnsNames<<record.fieldName(s);
    }
    couponsTable->setHorizontalHeaderLabels(columnsNames);
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

            couponsTable->setItem(row, column, item);
        }
        row++;

     }

    coupons::currentpage = 0;
    coupons::showPage(couponsTable);

}



bool coupons::addCouponToDB(QString code, QString totalNum, QString startDate, QString endDate, QString constraints){



    QSqlQuery query;
    query.prepare("INSERT INTO COUPONS (CODE, USES, STARTDATE, ENDDATE, CONSTRAINTS)"
                  "VALUES(:CODE, :USES, TO_DATE(:STARTDATE, 'YYYY-MM-DD hh24:mi:ss'), TO_DATE(:ENDDATE, 'YYYY-MM-DD hh24:mi:ss'), :CONSTRAINTS)");
    query.bindValue(":CODE",code);
    query.bindValue(":USES",totalNum);
    query.bindValue(":STARTDATE",startDate);
    query.bindValue(":ENDDATE",endDate);
    query.bindValue(":CONSTRAINTS",constraints);
    qDebug() << query.lastQuery();
    bool result = query.exec();




    return result;
}




bool coupons::editCouponData(QTableWidget* couponsTable, int row){



    QSqlQuery query;
    query.prepare("UPDATE COUPONS SET CODE= :CODE, STARTDATE= TO_DATE(:STARTDATE,'yyyy/mm/dd\"T\"hh24:mi:ss'), ENDDATE= TO_DATE(:ENDDATE,'yyyy/mm/dd\"T\"hh24:mi:ss'), USES= :USES, CONSTRAINTS= :CONSTRAINTS WHERE ID= :ID");

    query.bindValue(":ID",couponsTable->item(row,0)->text());
    query.bindValue(":CODE",couponsTable->item(row,1)->text());
    query.bindValue(":USES",couponsTable->item(row,5)->text());
    query.bindValue(":STARTDATE",couponsTable->item(row,2)->text());
    query.bindValue(":ENDDATE",couponsTable->item(row,3)->text());
    query.bindValue(":CONSTRAINTS",couponsTable->item(row,4)->text());
    qDebug()<<"id:"<<couponsTable->item(row,0)->text()<<" code:"<<couponsTable->item(row,1)->text()<<" uses:"<<couponsTable->item(row,5)->text()<<" dateS:"<<couponsTable->item(row,2)->text()<<" dateE:"<<couponsTable->item(row,3)->text()<<" const:"<<couponsTable->item(row,4)->text();


    return query.exec();
}






void coupons::sortAccording(QTableWidget* couponsTable,int logicalIndex)
{
     if(coupons::currentsorted != logicalIndex)
     {
        coupons::currentsorting = -1;
        coupons::currentsorted = logicalIndex;
     }
     coupons::currentsorting = (coupons::currentsorting + 1)%2;

    QString text = couponsTable->horizontalHeaderItem(logicalIndex)->text();
   qDebug() << logicalIndex << text;
   couponsTable->sortItems(logicalIndex,coupons::currentsorting==1?Qt::AscendingOrder:Qt::DescendingOrder);

}

void coupons::nextPage(QTableWidget* couponsTable)
{
    qDebug()<<"say smth";
    coupons::currentpage++;
    if(coupons::currentpage > couponsTable->rowCount()/coupons::maxPerPage)coupons::currentpage = couponsTable->rowCount()/coupons::maxPerPage;
    coupons::showPage(couponsTable);


}
void coupons::prevPage(QTableWidget* couponsTable)
{
    coupons::currentpage--;
    if(coupons::currentpage < 0)coupons::currentpage = 0;

    coupons::showPage(couponsTable);
}

void coupons::showPage(QTableWidget* couponsTable)
{

    for(int i=0; i<couponsTable->rowCount(); i++)
    {
        couponsTable->showRow(i);

        if(i > coupons::currentpage*coupons::maxPerPage + (coupons::maxPerPage-1) || i < coupons::currentpage*coupons::maxPerPage)
             couponsTable->hideRow(i);

    }


}






void coupons::searchText(QTableWidget* couponsTable, QString textsearched)
{
    qDebug()<<"eyo";
    map<int,bool> rowsFound;

    for (int row = 0 ; row < couponsTable->rowCount() ; ++row) {
    for (int col = 0 ; col < couponsTable->columnCount() ; ++col) {
        couponsTable->item(row, col)->setBackgroundColor(*new QColor(255,255,255));;
    }
    }

    if(textsearched != "")
    {
        foreach(QTableWidgetItem* item, couponsTable->findItems(textsearched,Qt::MatchContains))
        {
            item->setBackgroundColor(*new QColor(120,120,120));
            rowsFound[item->row()] = true;
        }

        for(int i=0; i<couponsTable->rowCount(); i++)
        {
            if(rowsFound[i])
                 {
                couponsTable->showRow(i);

            }
            else
            {
                  couponsTable->hideRow(i);
            }

        }
    }
    else
    {
        coupons::showPage(couponsTable);

    }

}




