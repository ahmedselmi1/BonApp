#include "queue.h"
#include <QDebug>




queue::queue(int id, QString recette, QString etat)
{
    this->id = id;
    this->recette = recette;
    this->etat = etat;
}




bool queue::ajouter()
{
    QSqlQuery query;

    QString res = QString::number(id);



    query.prepare("insert into fileattente (id,recette,etat)" "values (:id, :recette, :etat)");

    query.bindValue(":id", res);
    query.bindValue(":recette", recette);
    query.bindValue(":etat", etat);

    return query.exec();
}

QSqlQueryModel * queue::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();

    model->setQuery("select * from fileattente");
    model->setHeaderData(0,Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal, QObject::tr("Recette"));
    model->setHeaderData(2,Qt::Horizontal, QObject::tr("Etat"));

    return model;
}




bool queue::supprimer(int id)
{
    if(id==-1)
    {
        QSqlQuery query;
        query.prepare("Delete from fileattente where 1=1");
        return query.exec();
    }
    else
    {
        QSqlQuery query;
        QString res = QString::number(id);
        query.prepare("Delete from fileattente where id=:id");
        query.bindValue(":id", res);
        return query.exec();
    }
}

bool queue::modifier()
{
    QSqlQuery query;

    QString res = QString::number(id);


    query.prepare("update fileattente set recette=:recette, etat=:etat where id=:id");
    qDebug() << "res = " << res;
    query.bindValue(":id", res);
    query.bindValue(":recette", recette);
    query.bindValue(":etat", etat);

    return query.exec();
}










