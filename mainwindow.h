#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "materiel.h"
#include <QMainWindow>
#include "produit.h"
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


    QString getSearchText();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_6_clicked();

    void on_lineEdit_13_textChanged(const QString &arg1);



    void on_comboBox_currentIndexChanged(int index);



    void on_PDF_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::MainWindow *ui;
    materiel Mtmp;
    produit Ptmp;

};
#endif // MAINWINDOW_H
