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

    QString get_nom();
    QString get_adresse();
    int get_id();
    QString get_prenom();
    QString get_email();
    QString get_tel();

    //functions
    bool addClient();
    bool remove(QString);
    bool modify();
    static bool addClientToDB(QString,QString,QString,QString,QString);
    static void processClientTable(QTableWidget*);
    static void deleteSelectedClients(QTableWidget*);
    static void changeSelectedClientCell(QTableWidget*,int,int);
    static void printPDF(QTableWidget* clientsTable);


private:
    QString nom, adresse, prenom, email, tel;
    int id;
};

#endif // CLIENTS_H
