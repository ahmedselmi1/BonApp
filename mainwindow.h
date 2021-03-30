#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <employee.h>
#include <shifts.h>

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

    void on_empTable_activated(const QModelIndex &index);

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

    void on_toolButton_pdf_2_clicked();

private:
    Ui::MainWindow *ui;

    //louay doc
    employee tableEmployee, tableEmployee1;
    shifts tableShifts, tableShifts1;
};
#endif // MAINWINDOW_H
