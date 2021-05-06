#include "database.h"
#include <QDebug>
Database::Database()
{

}
bool Database::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("Karim");
db.setPassword("esprit18");
if (db.open()){
test=true;
qDebug() << "database opened";}else{
    qDebug()<< db.lastError();}
 return  test;
}
void Database::closeconnection(){
    db.close();
    qDebug() << "database closed";
    db.removeDatabase(QSqlDatabase::defaultConnection);
}
