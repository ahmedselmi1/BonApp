#ifndef RECETTE_H
#define RECETTE_H
#include<QSqlQuery>
#include<QSqlQueryModel>

class recette
{
    QString nom;
    int id,duree;
public:
    recette(){};
    recette(int,QString,int);

    QString getNom(){return nom;}
    int getDuree(){return duree;}
    int getID(){return id;}

    void setNom(QString n){nom = n;}
    void setDuree(int d){duree = d;}
    void setID(int nid){id = nid;}

    bool ajouter();
    bool modifier();
    QSqlQueryModel * afficher();
    bool supprimer(int);
};

#endif // RECETTE_H
