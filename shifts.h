#ifndef SHIFTS_H
#define SHIFTS_H
#include <QString>
#include <QTimeEdit>
#include <QSqlQuery>
#include <QSqlQueryModel>


class shifts
{
public:
    shifts();
    shifts(QString,QString,QString,QString,QString);

    QString get_cin();
    QString get_startday();
    QString get_endday();
    QString get_hstart();
    QString get_hend();

    bool add();   
    bool modify();
    bool remove(QString);
    QSqlQueryModel * show();
    QSqlQueryModel * show_Asc();
    QSqlQueryModel * show_Desc();
    QSqlQueryModel * search(const QString&);
    void show_notification(QString, QString);
private:
    QString cin, startday, endday,hstart,hend,titre,text;

};

#endif // SHIFTS_H
