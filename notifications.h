#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H


#include <QSystemTrayIcon>
#include<QString>
class Notifications
{
public:
    Notifications();
    void notifications_ajoutercadeau();
    void notifications_ajouterparticipation();
    void notifications_supprimerproduit();
    void notifications_supprimerparticipation();
    void notifications_modifiercadeau();
    void notifications_modifierparticipation();

};



#endif // NOTIFICATIONS_H
