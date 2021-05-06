#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "materiel.h"
#include <QMainWindow>
#include "produit.h"
#include "notifications.h"
#include "secdialog.h"
#include "database.h"
#include<QPrintDialog>
#include<QPrinter>
#include<QTextDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Database db;
    SecDialog *sec;
    Notifications N;

    QString getSearchText();

private slots:

    void on_pushbuttonmat_clicked();

    void on_pushbuttonprod_clicked();

    void on_pushbuttonback2_clicked();

    void on_pushbuttonback1_clicked();

    void on_pushButtonaj1_clicked();

    void on_pushButtonback_clicked();

    void on_pushButtonback3_clicked();

    void on_pushButtonaj2_clicked();

    void on_pushButtonval2_clicked();

    void on_pushButtonsup2_clicked();



    void on_pushButtonvalid1_clicked();

    void on_pushButtonsup1_clicked();



    void on_pushButtonmod1_clicked();

    void on_lineEditrech1_textChanged(const QString &arg1);



    void on_comboboxtri1_currentIndexChanged(int index);



    void on_PDF11_clicked();

    void on_stat11_clicked();

    void on_tableView11_activated(const QModelIndex &index);

    void on_pushButtonmod2_clicked();

    void on_PDF_mat_clicked();

    void on_pushButton_mat_clicked();


    void on_lineEdit_rech_mat_textChanged(const QString &arg1);

    void on_comboboxtri2_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    materiel Mtmp;
    produit Ptmp;

};
#endif // MAINWINDOW_H
