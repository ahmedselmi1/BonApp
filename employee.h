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
    QSqlQuery getStat_1(int &);
    QSqlQuery getStat_2(int &);
    QSqlQuery getStat_3(int &);
    QSqlQuery getStat_4(int &);
    QSqlQuery getStat_5(int &);
    QSqlQuery getStat_6(int &);
    QSqlQuery getStat_7(int &);
    bool add();
    bool modify();
    bool remove(QString toSearch);
    QSqlQueryModel * show();
    QSqlQueryModel * show_Asc();
    QSqlQueryModel * show_Desc();
    QSqlQueryModel * show_shifts();
    QSqlQueryModel * search(const QString&);
    void show_notification(QString, QString);   

private:
    QString  cin,fullname, email, address;
    int age, phone;
    //notification
    QString text,titre;
    int stat1, stat2,stat3,stat4,stat5,stat6,stat7;
};

#endif // EMPLOYEE_H
