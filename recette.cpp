#include "recette.h"

recette::recette(int id, QString nom, int duree)
{
    this->id = id;
    this->nom = nom;
    this->duree = duree;
}




bool recette::ajouter()
{
    QSqlQuery query;

    QString res = QString::number(id);
    QString res2 = QString::number(duree);


    query.prepare("insert into recettes (id,nom,duree)" "values (:id, :nom, :duree)");

    query.bindValue(":id", res);
    query.bindValue(":nom", nom);
    query.bindValue(":duree", res2);

    return query.exec();
}

QSqlQueryModel * recette::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();

    model->setQuery("select * from recettes");
    model->setHeaderData(0,Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2,Qt::Horizontal, QObject::tr("durée"));

    return model;
}




bool recette::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("Delete from recettes where id=:id");
    query.bindValue(":id", res);
    return query.exec();
}

bool recette::modifier()
{
    QSqlQuery query;

    QString res = QString::number(id);
    QString res2 = QString::number(duree);


    query.prepare("update recettes set nom=:nom, duree=:duree where id=:id");

    query.bindValue(":id", res);
    query.bindValue(":nom", nom);
    query.bindValue(":duree", res2);

    return query.exec();
}










