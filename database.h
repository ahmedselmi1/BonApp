#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Database
{
protected:
    QSqlDatabase db;
public:
    Database();
    bool createconnect();
    void closeconnection();

};

#endif // DATABASE_H
