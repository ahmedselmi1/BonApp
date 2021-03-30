#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include"connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;
    bool test=c.createconnect();
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful"), QMessageBox::Ok);
        w.show();
}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed"), QMessageBox::Ok);
    return a.exec();
}
