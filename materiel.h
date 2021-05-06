#ifndef MATERIEL_H
#define MATERIEL_H
#include <QString>
#include<QSqlQuery>
#include<QSqlQueryModel>

class materiel
{
 int nb_materiel,id_materiel;
 QString nom_materiel,date_entr;
public:
 materiel(){}
 materiel(int,int,QString,QString);
 int getid_materiel(){return id_materiel;}
 int getnb_materiel(){return nb_materiel;}
 QString getnom_materiel(){return nom_materiel;}
 QString getdate_entr(){return date_entr;}
 void setid_materiel(int id){this->id_materiel=id;}
 void setnb_materiel(int nb){this->nb_materiel=nb;}
 void setnom_materiel(QString nm){nom_materiel=nm;}
 void setdate_entr(QString de){date_entr=de;}
 bool ajouter();
 QSqlQueryModel * afficher(QString, int);
 bool supprimer(int);
 bool modify(int);
};

#endif // MATERIEL_H
