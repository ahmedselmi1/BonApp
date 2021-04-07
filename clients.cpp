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
#include <QAbstractItemModel>
using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;


using namespace std;


int clients::currentsorted = -1;
int clients::currentsorting = -1;
int clients::currentpage = 0;
int clients::maxPerPage = 8;

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
      QPrinter printer(QPrinter::PrinterResolution);
      printer.setOrientation(QPrinter::Portrait);
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setPaperSize(QPrinter::A4);
      printer.setOutputFileName("clients.pdf"); // will be in build folder


      QTextDocument doc;

      QString text("<center><img src='C:/Users/IyedBHD/Desktop/bonapp.png' width='150'><br><b>CLIENTS</b><br><style>table tr td:empty { width: 80px; } table tr td { padding-top: 10px; padding-bottom: 10px; }</style><table><thead>");
      text.append("<tr>");
      for (int i = 0; i < clientsTable->columnCount(); i++) {
          text.append("<th>").append(clientsTable->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append("</th>");
      }
      text.append("</tr></thead>");
      text.append("<tbody>");
      for (int i = 0; i < clientsTable->rowCount(); i++) {
          text.append("<tr>");
          for (int j = 0; j < clientsTable->columnCount(); j++) {
              QTableWidgetItem *item = clientsTable->item(i, j);
              if (!item || item->text().isEmpty()) {
                  clientsTable->setItem(i, j, new QTableWidgetItem("0"));
              }
              text.append("<td>").append(clientsTable->item(i, j)->text()).append("</td>");
          }
          text.append("</tr>");
      }
      text.append("</tbody></table></center>");
      doc.setHtml(text);
      doc.setPageSize(printer.pageRect().size());
      doc.print(&printer);

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


    clients::currentpage = 0;
    clients::showPage(clientsTable);
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


class MyTableWidgetItem : public QTableWidgetItem {
    public:
        bool operator <(const QTableWidgetItem &other) const
        {
            if(text().toInt() != NULL)
            return text().toInt() < other.text().toInt();
            else
            {

                return text() < other.text();
            }

        }
};

void clients::sortAccording(QTableWidget* clientsTable,int logicalIndex)
{
     if(clients::currentsorted != logicalIndex)
     {
        clients::currentsorting = -1;
        clients::currentsorted = logicalIndex;
     }
     clients::currentsorting = (clients::currentsorting + 1)%2;

    QString text = clientsTable->horizontalHeaderItem(logicalIndex)->text();
   qDebug() << logicalIndex << text;
   clientsTable->sortItems(logicalIndex,clients::currentsorting==1?Qt::AscendingOrder:Qt::DescendingOrder);

}

void clients::nextPage(QTableWidget* clientsTable)
{

    clients::currentpage++;
    if(clients::currentpage > clientsTable->rowCount()/clients::maxPerPage)clients::currentpage = clientsTable->rowCount()/clients::maxPerPage;
    clients::showPage(clientsTable);


}
void clients::prevPage(QTableWidget* clientsTable)
{
    clients::currentpage--;
    if(clients::currentpage < 0)clients::currentpage = 0;

    clients::showPage(clientsTable);
}

void clients::showPage(QTableWidget* clientsTable)
{

    for(int i=0; i<clientsTable->rowCount(); i++)
    {
        clientsTable->showRow(i);

        if(i > clients::currentpage*clients::maxPerPage + (clients::maxPerPage-1) || i < clients::currentpage*clients::maxPerPage)
             clientsTable->hideRow(i);

    }


}






void clients::searchText(QTableWidget* clientsTable, QString textsearched)
{
    map<int,bool> rowsFound;

    for (int row = 0 ; row < clientsTable->rowCount() ; ++row) {
    for (int col = 0 ; col < clientsTable->columnCount() ; ++col) {
        clientsTable->item(row, col)->setBackgroundColor(*new QColor(255,255,255));;
    }
    }

    if(textsearched != "")
    {
        foreach(QTableWidgetItem* item, clientsTable->findItems(textsearched,Qt::MatchContains))
        {
            item->setBackgroundColor(*new QColor(120,120,120));
            rowsFound[item->row()] = true;
        }

        for(int i=0; i<clientsTable->rowCount(); i++)
        {
            if(rowsFound[i])
                 {
                clientsTable->showRow(i);

            }
            else
            {
                  clientsTable->hideRow(i);
            }

        }
    }
    else
    {
        clients::showPage(clientsTable);

    }

    /*
    map<int,bool>::iterator it;

    for(it=rowsFound.begin(); it!=rowsFound.end(); ++it){

       }
       */

}






bool clients::editClientData(QTableWidget* clientsTable, int row){



    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET NOM= :NOM, PRENOM= :PRENOM, ADRESSE= :ADRESSE, TEL= :TEL WHERE ID= :ID");

    query.bindValue(":ID",clientsTable->item(row,0)->text());
    query.bindValue(":NOM",clientsTable->item(row,1)->text());
    query.bindValue(":PRENOM",clientsTable->item(row,2)->text());
    query.bindValue(":ADRESSE",clientsTable->item(row,3)->text());
    query.bindValue(":EMAIL",clientsTable->item(row,4)->text());
    query.bindValue(":TEL",clientsTable->item(row,5)->text());

    return query.exec();
}



