#include "clients.h"
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

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;


using namespace std;
clients::clients()
{

}

clients::clients(QString nom, QString adresse, int id, QString prenom, QString email, QString tel)
{

}

QString clients::get_nom(){return nom;}
QString clients::get_adresse(){return adresse;}
int clients::get_id(){return id;}
QString clients::get_prenom(){return prenom;}
QString clients::get_email(){return email;}
QString clients::get_tel(){return tel;}

bool clients::addClient(){
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT (ID, NOM, PRENOM, ADRESSE, EMAIL, TEL)"
                  "VALUES(:ID, :NOM, :PRENOM, :ADRESSE, :EMAIL, :TEL)");
    query.bindValue(":ID",id);
    query.bindValue(":NOM",nom);
    query.bindValue(":PRENOM",prenom);
    query.bindValue(":ADRESSE",adresse);
    query.bindValue(":EMAIL",email);
    query.bindValue(":TEL",tel);
    return query.exec();
}



bool clients::addClientToDB(QString nom, QString prenom, QString adresse, QString email, QString tel){

    QrCode qr = QrCode::encodeText("DamNNN!", QrCode::Ecc::MEDIUM);

    // Read the black & white pixels
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            int color = qr.getModule(x, y);  // 0 for white, 1 for black

            // You need to modify this part
            qDebug() << "draw pixel into "<<x<<" and "<<y<<" color: "<<color;

            //draw_pixel_onto_QT(x, y, color);
        }
    }



    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    qDebug() << mailREX.exactMatch("me@me.com");

    if(!mailREX.exactMatch(email))
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                    QObject::tr("Wrong Email Format"), QMessageBox::Ok);
        return false;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT (NOM, PRENOM, ADRESSE, EMAIL, TEL)"
                  "VALUES(:NOM, :PRENOM, :ADRESSE, :EMAIL, :TEL)");
    query.bindValue(":NOM",nom);
    query.bindValue(":PRENOM",prenom);
    query.bindValue(":ADRESSE",adresse);
    query.bindValue(":EMAIL",email);
    query.bindValue(":TEL",tel);
    bool result = query.exec();




    return result;
}


void clients::printPDF(QTableWidget* clientsTable)
{
    QPixmap pix(clientsTable->size());
      QPainter painter(&pix);
      clientsTable->render(&painter);
      painter.end();
      QPrinter printer(QPrinter::HighResolution);
      printer.setOrientation(QPrinter::Landscape);
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setPaperSize(QPrinter::A4);
      printer.setOutputFileName("clients.pdf"); // will be in build folder

      painter.begin(&printer);
      double xscale = printer.pageRect().width() / double(pix.width());
      double yscale = printer.pageRect().height() / double(pix.height());
      double scale = qMin(xscale, yscale);
      painter.translate(printer.paperRect().x() + printer.pageRect().width() / 2,
                        printer.paperRect().y() + printer.pageRect().height() / 2);
      painter.scale(scale, scale);
      painter.translate(-clientsTable->width() / 2, -clientsTable->height() / 2);
      painter.drawPixmap(0, 0, pix);

    QTextDocument doc;

    doc.setHtml("");
    doc.drawContents(&painter);

      painter.end();

}

void clients::processClientTable(QTableWidget* clientsTable)
{

    QSqlQuery query("SELECT ID, NOM, PRENOM, ADRESSE, EMAIL, TEL FROM CLIENT");
    clientsTable->clear();

    int numberOfRows = 0;
    if(query.last())
    {
        numberOfRows =  query.at() + 1;
        query.first();
        query.previous();
    }

    clientsTable->setRowCount(numberOfRows);

    int columns = query.record().count();
    QSqlRecord record = query.record();
    clientsTable->setColumnCount(columns);
    QStringList columnsNames = QStringList();
    for(int s=0;s<columns;s++)
    {
        columnsNames<<record.fieldName(s);
    }
    clientsTable->setHorizontalHeaderLabels(columnsNames);
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
            clientsTable->setItem(row, column, item);

        }
        row++;

     }


}



void clients::deleteSelectedClients(QTableWidget * clientsTable)
{

    map<QString,bool> toDelete;

    foreach(QTableWidgetItem* item, clientsTable->selectedItems())
    {
        toDelete[clientsTable->item(item->row(),0)->text()] = true;
    }
    map<QString,bool>::iterator it;

    for(it=toDelete.begin(); it!=toDelete.end(); ++it){
            QSqlQuery query;
            query.prepare("DELETE FROM CLIENT WHERE ID = :ID");
            query.bindValue(":ID", it->first);
            query.exec();
          //cout << it->first << " => " << it->second << '\n';
       }
    processClientTable(clientsTable);
    string aaa = std::to_string(toDelete.size()) + " Clients deleted successfully";
    QMessageBox::information(nullptr, QObject::tr("Clients Deleted"),
                      QObject::tr(aaa.c_str()), QMessageBox::Ok);



}


bool clients::remove(QString id)
{
QSqlQuery query;
query.prepare("DELETE FROM EMPLOYEE WHERE ID = :ID");
query.bindValue(":ID", id);
return    query.exec();
}

bool clients::modify(){
    QSqlQuery query;

    return query.exec();
}


void changeSelectedClientCell(QTableWidget* clientsTable,int row, int column)
{
    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET NOM= :NOM, PRENOM= :PRENOM, ADRESSE= :ADRESSE, TEL= :TEL WHERE ID= :ID");

    query.bindValue(":ID",clientsTable->item(row,0)->text());
    query.bindValue(":NOM",clientsTable->item(row,1)->text());
    query.bindValue(":PRENOM",clientsTable->item(row,2)->text());
    query.bindValue(":ADRESSE",clientsTable->item(row,3)->text());
    query.bindValue(":EMAIL",clientsTable->item(row,4)->text());
    query.bindValue(":TEL",clientsTable->item(row,5)->text());
    query.exec();
}
