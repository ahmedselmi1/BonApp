#include "materiel.h"

materiel::materiel(int nb_materiel,int id_materiel,QString nom_materiel,QString date_entr)
{
    this->id_materiel=id_materiel;
    this->nb_materiel=nb_materiel;
    this->nom_materiel=nom_materiel;
    this->date_entr=date_entr;
}
bool materiel::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(id_materiel);
    QString res2 = QString::number(nb_materiel);
    query.prepare("insert into materiel(id_materiel,nom_materiel,nb_materiel,date_entr)""values(:id_materiel,:nom_materiel,:nb_materiel,:date_entr)");
   query.bindValue(":id_materiel",res);
   query.bindValue(":nom_materiel",nom_materiel);
   query.bindValue(":nb_materiel",res2);
   query.bindValue(":date_entr",date_entr);
   return query.exec();
}
QSqlQueryModel * materiel::afficher()
{
QSqlQueryModel * model=new QSqlQueryModel();
model->setQuery("select * from materiel");
model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_materiel"));

model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom_materiel"));

model->setHeaderData(2,Qt::Horizontal,QObject::tr("date_entr"));

model->setHeaderData(3,Qt::Horizontal,QObject::tr("nb_materiel"));
return model;
}
bool materiel::supprimer(int id_materiel)
{
QSqlQuery query;
QString res = QString::number(id_materiel);
query.prepare("Delete from materiel where id_materiel= :id_materiel");
query.bindValue(":id_materiel",res);
return query.exec();
}




bool materiel::modify(int id_materiel){
    QSqlQuery query;
    query.prepare("UPDATE MATERIEL SET ID= :id_materiel, nom materiel= :nom_materiel, nbr materiel= :nb_materiel, date entretien= :date_entr WHERE ID= :id_materiel");
    query.bindValue(":id_materiel",id_materiel);
    query.bindValue(":nom_materiel",nom_materiel);
    query.bindValue(":nb_materiel",nb_materiel);
    query.bindValue(":date_entr",date_entr);
    return query.exec();
}











