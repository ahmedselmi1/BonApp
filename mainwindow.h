#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <employee.h>
#include <shifts.h>
#include "role.h"
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>

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

    void pdf();

    void on_goToEmployee_clicked();

    void on_back_5_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

    void on_back_8_clicked();

    void on_back_9_clicked();

    void on_toolButton_save_2_clicked();

    void on_toolButton_10_clicked();

    void on_toolButton_modify_clicked();

    void on_back_10_clicked();

    void on_empTable_activated();

    void on_toolButton_5_clicked();

    void on_toolButton_7_clicked();

    void on_toolButton_save_3_clicked();

    void on_back_15_clicked();

    void on_toolButton_11_clicked();

    void on_toolButton_12_clicked();

    void on_toolButton_13_clicked();

    void on_back_16_clicked();

    void on_back_17_clicked();

    void on_toolButton_6_clicked();

    void on_empTable_clicked(const QModelIndex &index);

    void on_emptable_2_clicked(const QModelIndex &index);

    void on_shiftsTab_clicked(const QModelIndex &index);

    void on_toolButton_delete_4_clicked();

    void on_back_18_clicked();

    void on_shiftsTab_doubleClicked(const QModelIndex &index);

    void on_toolButton_edit_6_clicked();

    void on_toolButton_delete_5_clicked();

    void on_toolButton_delete_6_clicked();

    void on_toolButton_delete_7_clicked();

    void on_toolButton_pdf_clicked();

    void on_back_11_clicked();

    void on_toolButton_modify_2_clicked();

    void on_toolButton_delete_8_clicked();

    void on_toolButton_8_clicked();

    void on_gotoRole_clicked();

    void on_nameRole_activated(const QString &arg1);

    void on_addRoleButton_clicked();

    void on_previousPage_clicked();

    void on_nextPage_clicked();

    void on_deleteRole_clicked();

    void on_roleTable_cellClicked(int row);

    void on_editRole_clicked();

    void on_modifyRoleButton_clicked();

    void on_search_clicked();

    void on_sortRole_clicked();

    void on_sortEarning_clicked();

    void on_back_12_clicked();

    void on_back_13_clicked();

    void on_back_14_clicked();

    void on_back_19_clicked();

    void on_stats_clicked();

private:

Ui::MainWindow *ui;

    employee tableEmployee, tableEmployee1;
    shifts tableShifts, tableShifts1;
    role Role;
};
#endif // MAINWINDOW_H
