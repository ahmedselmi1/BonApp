#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>


class employee
{
public:
    employee();
    employee(QString,QString,int,QString,int,QString);

    QString get_cin();
    QString get_fullname();
    int get_age();
    QString get_email();
    int get_phone();
    QString get_address();

    //functions
    bool add();
    bool modify();
    bool remove(QString);
    bool pdfunction(QString &,QString &);
    QSqlQueryModel * show();
    QSqlQueryModel * show_Asc();
    QSqlQueryModel * show_Desc();
    QSqlQueryModel * show_shifts();
    QSqlQueryModel * search(const QString&);
    //notification();
    //notification(QString,QString);
    void show_notification(QString, QString);


private:
    QString  cin,fullname, email, address;
    int age, phone;
    //notification
    QString text,titre;
};

#endif // EMPLOYEE_H
