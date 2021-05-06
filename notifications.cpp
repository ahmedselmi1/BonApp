#include "notifications.h"
#include <QSystemTrayIcon>
#include<QString>
Notifications::Notifications()
{

}

void Notifications::notifications_ajoutercadeau()
{

    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

   // notifyIcon->setIcon(QIcon(":/new/prefix1/MyResources/computer-icons-avatar-user-login-avatar.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des cadeaux ","Nouveau cadeau ajouté ",QSystemTrayIcon::Information,15000);
}
void Notifications::notifications_ajouterparticipation()
{

    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

   // notifyIcon->setIcon(QIcon(":/new/prefix1/MyResources/computer-icons-avatar-user-login-avatar.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des participation ","Nouvelle participation ajouté ",QSystemTrayIcon::Information,15000);
}
void Notifications::notifications_supprimerproduit(){
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

   // notifyIcon->setIcon(QIcon(":/new/prefix1/MyResources/computer-icons-avatar-user-login-avatar.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des produits ","Produit suprimé",QSystemTrayIcon::Information,15000);
}
void Notifications::notifications_supprimerparticipation(){
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

   // notifyIcon->setIcon(QIcon(":/new/prefix1/MyResources/computer-icons-avatar-user-login-avatar.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des participations ","Une participation est supprimé",QSystemTrayIcon::Information,15000);

}
void Notifications::notifications_modifiercadeau(){
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

   // notifyIcon->setIcon(QIcon(":/new/prefix1/MyResources/computer-icons-avatar-user-login-avatar.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des cadeaux ","Un cadeau est modifié",QSystemTrayIcon::Information,15000);

}
void Notifications::notifications_modifierparticipation(){
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

   // notifyIcon->setIcon(QIcon(":/new/prefix1/MyResources/computer-icons-avatar-user-login-avatar.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des participations ","Une participation est modifié",QSystemTrayIcon::Information,15000);

}
