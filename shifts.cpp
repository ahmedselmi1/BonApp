#include "shifts.h"
#include "employee.h"

shifts::shifts()
{
    cin="";
    jours="";
    hdeb="";
    hfin="";
}

shifts::shifts(QString cin, QString jours, QString hdeb, QString hfin)
{
    this->cin=cin;
    this->jours=jours;
    this->hdeb=hdeb;
    this->hfin=hfin;
}

QString shifts::get_cin(){return cin;}
QString shifts::get_jours(){return jours;}
QString shifts::get_hdeb(){return hdeb;}
QString shifts::get_hfin(){return hfin;}

bool shifts::add(){
    QSqlQuery query;
    query.prepare("INSERT INTO SHIFTS(CIN, JOURS, HDEB, HFIN)"
                  "VALUES(:CIN, :JOURS, :HDEB, :HFIN)");
    query.bindValue(":CIN",cin);
    query.bindValue(":JOURS",jours);
    query.bindValue(":HDEB",hdeb);
    query.bindValue(":HFIN",hfin);
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
    query.prepare("UPDATE SHIFTS SET CIN= :CIN, JOURS= :JOURS, HDEB= :HDEB, HFIN= :HFIN WHERE CIN= :CIN");
    query.bindValue(":CIN",cin);
    query.bindValue(":JOURS",jours);
    query.bindValue(":HDEB",hdeb);
    query.bindValue(":HFIN",hfin);
    return query.exec();
}


