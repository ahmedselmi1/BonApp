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
    shifts(QString,QString,QString,QString);

    QString get_cin();
    QString get_jours();
    QString get_hdeb();
    QString get_hfin();

    bool add();
    bool remove(QString);
    bool modify();
    QSqlQueryModel * show();    

private:
    QString cin, jours, hdeb,hfin;

};

#endif // SHIFTS_H
