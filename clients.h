#ifndef CLIENTS_H
#define CLIENTS_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>
using namespace std;
class clients
{
public:
    clients();
    clients(QString,QString,int,QString,QString,QString);
    static QString yo;
    QString get_nom();
    QString get_adresse();
    int get_id();
    QString get_prenom();
    QString get_email();
    QString get_tel();
    static int currentsorted;
    static int currentsorting;
    static int currentpage;
    static int maxPerPage;

    //functions
    bool addClient();
    bool remove(QString);
    bool modify();
    static bool addClientToDB(QString,QString,QString,QString,QString);
    static void processClientTable(QTableWidget*);
    static void deleteSelectedClients(QTableWidget*);
    static void changeSelectedClientCell(QTableWidget*,int,int);
    static void printPDF(QTableWidget* clientsTable);
    static void sortAccording(QTableWidget* clientsTable,int logicalIndex);
    static void nextPage(QTableWidget* clientsTable);
    static void prevPage(QTableWidget* clientsTable);
    static void showPage(QTableWidget* clientsTable);
    static void searchText(QTableWidget* clientsTable, QString textsearched);
    static bool editClientData(QTableWidget* clientsTable, int row);

private:
    QString nom, adresse, prenom, email, tel;
    int id;
};

#endif // CLIENTS_H
