#ifndef CATEGORY_H
#define CATEGORY_H
#include <QString>
#include <QSqlQuery>
#include <QSystemTrayIcon>
#include <QSqlQueryModel>

class category
{
public:
    category();
    category(QString,QString);
    QString get_id();
    QString get_type();
    bool add_category();
    QSqlQuery stat_1(int &);
    QSqlQuery stat_2(int &);
    QSqlQuery stat_3(int &);
    //func
    QSqlQueryModel * show();
    bool remove(int);
    bool modify();
    QSqlQueryModel * search(const QString&);
    QSqlQueryModel * show_Desc();
    void show_notification(QString, QString);

private:
    QString id,type,text,titre;

};



#endif // CATEGORY_H
