#ifndef QUEUE_H
#define QUEUE_H
#include<QSqlQuery>
#include<QSqlQueryModel>


class queue
{
    QString recette, etat;
    int id;
public:
    queue(){};
    queue(int,QString,QString);

    QString getRecette(){return recette;}
    QString getEtat(){return etat;}
    int getID(){return id;}

    void setRecette(QString n){recette = n;}
    void setEtat(QString d){etat = d;}
    void setID(int nid){id = nid;}

    bool ajouter();
    bool modifier();
    QSqlQueryModel * afficher();
    bool supprimer(int);
};

#endif // QUEUE_H
