#ifndef PRODUIT_H
#define PRODUIT_H
#include <QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QFile>
#include <QFileDialog>
class produit
{
 int quantite_ingredient,id_ingrediant;
 QString nom_ingredient,date_exp;
public:
 produit(){}
 produit(int,int,QString,QString);
 int getid_ingrediant(){return id_ingrediant;}
 int getquantite_ingredient(){return quantite_ingredient;}
 QString getnom_ingredient(){return nom_ingredient;}
 QString getdate_exp(){return date_exp;}
 void setid_ingrediant(int id){this->id_ingrediant=id;}
 void setquantite_ingredient(int qi){this->quantite_ingredient=qi;}
 void setnom_ingredient(QString ni){nom_ingredient=ni;}
 void setdate_exp(QString dex){date_exp=dex;}
 bool ajouter();
 QSqlQueryModel * afficher(QString, int);
 bool supprimer(int);
 bool modify(int);
 int stati();
 int stati1();
 int stati2();

 int nb_total();


};

#endif // PRODUIT_H
