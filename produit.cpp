#include "produit.h"

produit::produit( int quantite_ingredient,int id_ingrediant,QString nom_ingredient,QString date_exp)
{
    this->id_ingrediant=id_ingrediant;
    this->quantite_ingredient=quantite_ingredient;
    this->nom_ingredient=nom_ingredient;
    this->date_exp=date_exp;
}
bool produit::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(id_ingrediant);
    QString res2 = QString::number(quantite_ingredient);
    query.prepare("insert into produit(id_ingrediant,nom_ingredient,quantite_ingredient,date_exp)""values(:id_ingrediant,:nom_ingredient,:quantite_ingredient,:date_exp)");
   query.bindValue(":id_ingrediant",res);
   query.bindValue(":nom_ingredient",nom_ingredient);
   query.bindValue(":quantite_ingredient",res2);
   query.bindValue(":date_exp",date_exp);
   return query.exec();
}
QSqlQueryModel * produit::afficher(QString rech="", int ord=0)
{
QSqlQueryModel * model=new QSqlQueryModel();
model->setQuery("select id_ingrediant,nom_ingredient,date_exp,quantite_ingredient from produit where lower(nom_ingredient) LIKE lower('%" + rech + "%') ORDER BY " + (ord>1?"nom_ingredient":"id_ingrediant") + " " + (ord%2==0?"ASC":"DESC"));
model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_ingrediant"));

model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom_ingredient"));

model->setHeaderData(2,Qt::Horizontal,QObject::tr("date_exp"));

model->setHeaderData(3,Qt::Horizontal,QObject::tr("quantite_ingredient"));
return model;
}
bool produit::supprimer(int id_ingrediant)
{
QSqlQuery query;
QString res = QString::number(id_ingrediant);
query.prepare("Delete from produit where id_ingrediant= :id_ingrediant");
query.bindValue(":id_ingrediant",res);
return query.exec();
}
bool produit::modify(int id_ingrediant){
    QSqlQuery query;
    query.prepare("UPDATE produit SET ID= :id_ingrediant, nom_ingredient= :nom_ingredient, quantite ingredient= :quantite_ingredient, date exp= :date_exp WHERE ID= :id_ingrediant");
    query.bindValue(":id_ingrediant",id_ingrediant);
    query.bindValue(":nom_ingredient",nom_ingredient);
    query.bindValue(":quantite_ingredient",quantite_ingredient);
    query.bindValue(":date_exp",date_exp);
    return query.exec();
}
