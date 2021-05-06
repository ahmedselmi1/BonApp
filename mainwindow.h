#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recette.h"
#include "queue.h"
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_recettesBtn_clicked();

    void on_queueBtn_clicked();





    void on_rafficherTable_customContextMenuRequested(const QPoint &pos);

    void slotAddBefore();
    void slotAddAfter();
    void slotAddEnd();
    void slotDeleteRow();


    void on_lineEdit111_textChanged(const QString &arg1);


    void on_toolButton111_clicked();

    void on_pushButtonpdf_clicked();

    void on_pushButtonrec1_clicked();

    void on_pushButtonrec3_clicked();

    void on_pushButtonrec4_clicked();

    void on_queueTableView_pressed(const QModelIndex &index);

    void on_queueTableView_clicked(const QModelIndex &index);

    void on_pushButtonrec2_clicked();

    void on_returnbtn1_clicked();

    void on_returnbtn1_2_clicked();

    void on_pushButtonrec3_pressed();

private:
    Ui::MainWindow *ui;

    QSqlTableModel *tableModel;

    bool rmodif = false;

    bool isAscending = false;

    recette Rtmp;
    queue Qtmp;
    int lastQueue = -1;
};
#endif // MAINWINDOW_H
