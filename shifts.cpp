#include "shifts.h"
#include <QSystemTrayIcon>

shifts::shifts()
{
    cin="";
    startday="";
    endday="";
    hstart="";
    hend="";
}

shifts::shifts(QString cin, QString startday, QString endday, QString hstart, QString hend)
{
    this->cin=cin;
    this->startday=startday;
    this->endday=endday;
    this->hstart=hstart;
    this->hend=hend;
}

QString shifts::get_cin(){return cin;}
QString shifts::get_startday(){return startday;}
QString shifts::get_endday(){return endday;}
QString shifts::get_hstart(){return hstart;}
QString shifts::get_hend(){return hend;}

bool shifts::add(){
    QSqlQuery query;
    query.prepare("INSERT INTO SHIFTS(CIN, STARTDAY, ENDDAY, HSTART, HEND)"
                  "VALUES(:CIN, :STARTDAY, :ENDDAY, :HSTART, :HEND)");
    query.bindValue(":CIN",cin);
    query.bindValue(":STARTDAY",startday);
    query.bindValue(":ENDDAY",endday);
    query.bindValue(":HSTART",hstart);
    query.bindValue(":HEND",hend);
    return query.exec();
}

bool shifts::remove(QString cin)
{
QSqlQuery query;
QString toSearch= cin;
query.prepare("DELETE FROM SHIFTS WHERE CIN = :CIN");
query.bindValue(":CIN", toSearch);
return    query.exec();
}

bool shifts::modify(){
    QSqlQuery query;
    query.prepare("UPDATE SHIFTS SET CIN= :CIN, STARTDAY= :STARTDAY, ENDDAY= :ENDDAY, HSTART= :HSTART , HEND= :HEND  WHERE CIN= :CIN");
    query.bindValue(":CIN",cin);
    query.bindValue(":STARTDAY",startday);
    query.bindValue(":ENDDAY",endday);
    query.bindValue(":HSTART",hstart);
    query.bindValue(":HEND",hend);
    return query.exec();
}

QSqlQueryModel * shifts::show()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT EMPLOYEE.CIN, EMPLOYEE.FULLNAME, SHIFTS.STARTDAY, SHIFTS.ENDDAY, SHIFTS.HSTART, SHIFTS.HEND FROM EMPLOYEE INNER JOIN SHIFTS ON EMPLOYEE.CIN=SHIFTS.CIN");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Starts on :"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Ends on :"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Starts at :"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Ends at :"));
        return model;
}

QSqlQueryModel * shifts::search(const QString &startday)
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("SELECT EMPLOYEE.CIN, EMPLOYEE.FULLNAME, SHIFTS.STARTDAY, SHIFTS.ENDDAY, EMPLOYEE.PHONE FROM EMPLOYEE FULL OUTER JOIN SHIFTS ON EMPLOYEE.CIN=SHIFTS.CIN where (STARTDAY LIKE '"+startday+"%') ");
     model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("Start Day"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("End Day"));
     model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone"));
     return model;
}

QSqlQueryModel * shifts::show_Asc()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT EMPLOYEE.CIN, EMPLOYEE.FULLNAME, SHIFTS.STARTDAY, SHIFTS.ENDDAY, SHIFTS.HSTART, SHIFTS.HEND FROM EMPLOYEE INNER JOIN SHIFTS ON EMPLOYEE.CIN=SHIFTS.CIN ORDER BY SHIFTS.STARTDAY ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Starts on :"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Ends on :"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Starts at :"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Ends at :"));
        return model;
}

QSqlQueryModel * shifts::show_Desc()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT EMPLOYEE.CIN, EMPLOYEE.FULLNAME, SHIFTS.STARTDAY, SHIFTS.ENDDAY, SHIFTS.HSTART, SHIFTS.HEND FROM EMPLOYEE INNER JOIN SHIFTS ON EMPLOYEE.CIN=SHIFTS.CIN ORDER BY SHIFTS.STARTDAY DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Starts on :"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Ends on :"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Starts at :"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Ends at :"));
        return model;
}

void shifts::show_notification(QString titre,QString text)
{
    this->text=text;
      this->titre=titre;
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
    notifyIcon->setIcon(QIcon(":/img/img/logo.png"));
    notifyIcon->show();
    notifyIcon->showMessage(titre,text,QSystemTrayIcon::Information,15000);
}
