#include "shifts.h"
#include <QSystemTrayIcon>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>

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

QSqlQuery shifts::getStat_1(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Mon%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery shifts::getStat_2(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Tue%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery shifts::getStat_3(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Wed%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery shifts::getStat_4(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Thu%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery shifts::getStat_5(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Fri%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery shifts::getStat_6(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Sat%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}

QSqlQuery shifts::getStat_7(int &stat)
{
    stat = 0;
    QSqlQuery query("SELECT COUNT(*) FROM SHIFTS WHERE STARTDAY LIKE 'Sun%'");
    while(query.next())
    {
        stat = query.value(0).toInt();
    }
    return query;
}
void shifts::stats(QFrame *shifts)
{
    int stat1,stat2,stat3,stat4,stat5,stat6,stat7;

        shifts::getStat_1(stat1);
        shifts::getStat_2(stat2);
        shifts::getStat_3(stat3);
        shifts::getStat_4(stat4);
        shifts::getStat_5(stat5);
        shifts::getStat_6(stat6);
        shifts::getStat_7(stat7);
        QPieSeries *series = new QPieSeries();

        series->append("Monday",stat1);
        series->append("Tuesday",stat2);
        series->append("Wednesday",stat3);
        series->append("Thursday",stat4);
        series->append("Friday",stat5);
        series->append("Saturday",stat6);
        series->append("Sunday",stat7);


        series->setHoleSize(0.5);
        series->setPieSize(0.8);

        QPieSlice *Monday = series->slices().at(0);
        QPieSlice *Tuesday = series->slices().at(1);
        QPieSlice *Wednesday = series->slices().at(2);
        QPieSlice *Thursday = series->slices().at(3);
        QPieSlice *Friday = series->slices().at(4);
        QPieSlice *Saturday = series->slices().at(5);
        QPieSlice *Sunday = series->slices().at(6);

        Monday->setLabelVisible(true);
        Tuesday->setLabelVisible(true);
        Wednesday->setLabelVisible(true);
        Thursday->setLabelVisible(true);
        Friday->setLabelVisible(true);
        Saturday->setLabelVisible(true);
        Sunday->setLabelVisible(true);

        Monday->setBrush(QColor::fromRgb(205, 165, 243));
        Tuesday->setBrush(QColor::fromRgb(185, 129, 239));
        Wednesday->setBrush(QColor::fromRgb(166, 93, 234));
        Thursday->setBrush(QColor::fromRgb(146, 56, 229));
        Friday->setBrush(QColor::fromRgb(105, 23, 181));
        Saturday->setBrush(QColor::fromRgb(105, 23, 181));
        Sunday->setBrush(QColor::fromRgb(84, 19, 144));

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Working days");
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->setAnimationOptions(QChart::AllAnimations);

        QChartView * chartview = new QChartView(chart);
        chartview->setParent(shifts);
}
