#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recette.h"
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

    void on_rajouterBtn_clicked();

    void on_recettesTab_currentChanged(int index);

    void on_rsupprimerBtn_clicked();

    void on_rafficherTable_doubleClicked(const QModelIndex &index);

    void on_rafficherTable_customContextMenuRequested(const QPoint &pos);

    void slotAddBefore();
    void slotAddAfter();
    void slotAddEnd();
    void slotDeleteRow();


    void on_lineEdit_textChanged(const QString &arg1);


    void on_toolButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlTableModel *tableModel;

    bool rmodif = false;

    bool isAscending = false;

    recette Rtmp;
};
#endif // MAINWINDOW_H
