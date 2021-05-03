#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <employee.h>
#include <clients.h>
#include "arduino.h"
#include <deque>
#include <QtScript/QtScript>
#include "gateauxc.h"
#include "commandec.h"
#include "category.h"
#include <QPieSeries>
#include <QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg);
    static Ui::MainWindow getUi();
private slots:
    void on_goToOrders_clicked();

    void on_goToProducts_clicked();

    void on_goToStock_clicked();

    void onMsg();

    void on_goToClient_clicked();

    void on_goToEmployee_clicked();

    void on_back_1_clicked();

    void on_back_2_clicked();

    void on_back_3_clicked();

    void on_back_4_clicked();

    void on_back_5_clicked();

    void on_addProduct_clicked();

    void on_showProducts_clicked();

    void on_back_6_clicked();

    void on_back_7_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

    void on_back_8_clicked();

    void on_back_9_clicked();

    void on_toolButton_save_2_clicked();

    void on_toolButton_10_clicked();

    void on_toolButton_11_clicked();

    void on_toolButton_modify_clicked();

    void on_back_10_clicked();

    void on_addProduct_2_clicked();

    void on_showProducts_2_clicked();

    void on_back_11_clicked();

    void on_back_12_clicked();

    void on_toolButton_12_clicked();

    void on_toolButton_17_clicked();

    void on_back_13_clicked();

    void on_back_14_clicked();

    void on_gestionClient_clicked();

    void on_gestionCoupon_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_backbtn_2_clicked();

    void on_backbtn_clicked();

    void on_ajoutercoupon_clicked();

    void on_ajouterclient_clicked();

    void on_backbtn_3_clicked();

    void on_backbtn_4_clicked();

    void on_supprimerClient_clicked();

    void on_supprimerCoupon_clicked();

    void on_ajouterClientBtn_clicked();



    void on_clientsTable_cellChanged(int row, int column);


    void on_clientsTable_cellDoubleClicked(int row, int column);

    void on_labelClientNom_editingFinished();

    void on_labelClientNom_textEdited(const QString &arg1);

    void on_ajoutCoupon_clicked();

    void on_labelCouponCode_textChanged(const QString &arg1);

    void on_couponsTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_couponsTable_cellChanged(int row, int column);

    void on_couponsTable_cellDoubleClicked(int row, int column);

    void on_printtable1_clicked();

    void on_printtable2_clicked();

    void on_userSearch_textChanged();

    void on_clientsTable_cellClicked(int row, int column);

    void on_toolButton_5_clicked();

    void on_toolButton_6_clicked();

    void on_prevCoupon_clicked();

    void on_couponSearch_textChanged();

    void on_nextCoupon_clicked();

    void on_backvitrine_clicked();

    void on_toolButton_save_clicked();


    void on_toolButton_delete_clicked();

    void on_toolButton_savemod_clicked();

    void on_GatTab_activated(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_tableC_activated(const QModelIndex &index);

    void on_pushButton_deletec_clicked();

    void on_comboBox_name_currentIndexChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_savemod_clicked();

    void on_pushButton_calc_clicked();

    void on_comboBox_name_1_currentIndexChanged(const QString &arg1);

    void on_pushButton_search_2_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_sort_2_clicked();

    void on_pushButton_sort_clicked();

    void on_pushButton_pdf_clicked();

    void on_pushButton_ex_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_stat_clicked();

    void on_back_15_clicked();

    void on_back_16_clicked();

    void on_back_17_clicked();

    void on_back_18_clicked();

    void on_back_19_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_addCategory_clicked();

    void on_showCategory_clicked();

    void on_pushButton_10_clicked();

    void on_CatTab_activated(const QModelIndex &index);

    void on_back_20_clicked();

    void on_back_22_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_vitrineBtn_clicked();

    void on_musicNext_clicked();

    void on_musicBack_clicked();

    void on_musicStop_clicked();

    void on_musicPlay_clicked();

private:


    Ui::MainWindow *ui;

    //louay doc
    employee tableEmployee;
    clients Clients;
    pair<int,int> lastClientCell = make_pair(-1,-1);
    pair<int,int> lastCouponsCell = make_pair(-1,-1);
    deque<int> lastPlateValues;
    QByteArray data; // variable contenant les données reçues
    void animateObj(QWidget* widget, int current=255);
    void animateObjWhite(QWidget* widget, int current);

    Arduino A; // objet temporaire



    //GHADA
    gateauxC tabGateaux;
    commandeC tablecommande;
    category tabCategory;
/////////

protected:
    void timerEvent(QTimerEvent *event);


};
#endif // MAINWINDOW_H
