#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
db = QSqlDatabase::addDatabase("QODBC");
/*db.setDatabaseName("Source_Projet2A");
db.setUserName("ahmed");
db.setPassword("esprit20");*/

db.setDatabaseName("testdb1");//inserer le nom de la source de données ODBC
db.setUserName("iyed");//inserer nom de l'utilisateur
db.setPassword("daha");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;
 return  test;
}
void Connection::closeConnection(){db.close();}
