#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recette.h"
#include <QMessageBox>
#include <QDebug>
#include<QFileDialog>
#include<QPrinter>
#include<QTextDocument>
#include"dateformatdelegate.h"
#include "queue.h"
#include <QSound>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    tableModel = new QSqlTableModel();
    tableModel->setEditStrategy(QSqlTableModel::OnRowChange);
    tableModel->setTable("RECETTES");
    tableModel->select();
    ui->rafficherTable->setModel(tableModel);
    //ui->rafficherTable->setModel(Rtmp.afficher());
    ui->queueTableView->setModel(Qtmp.afficher());

    ui->rafficherTable->setColumnHidden(3, true);
    ui->rafficherTable->setItemDelegateForColumn(2, new DateFormatDelegate(this));

    auto header = ui->rafficherTable->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, [this](int logicalIndex){
        QString text = ui->rafficherTable->model()->headerData(logicalIndex, Qt::Horizontal).toString();
        isAscending = !isAscending;
        tableModel->sort(logicalIndex, isAscending?Qt::AscendingOrder:Qt::DescendingOrder);
       qDebug() << logicalIndex << text;
    });
    QSound::play(":/startup.wav");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_recettesBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QApplication::beep();
}

void MainWindow::on_queueBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QApplication::beep();
}




void MainWindow::slotDeleteRow()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){

        if (QMessageBox::warning(this,
                                 tr("Supprimer ligne"),
                                 tr("Etes vous sur de bien vouloir supprimer cette ligne définitivement ??"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            qDebug() << "Deleting row " << row;
            tableModel->removeRows(row, 1);
            tableModel->submitAll();
            return;
        }
    }
}

void MainWindow::slotAddBefore()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){
        tableModel->insertRows(row, 1);
    }
}


void MainWindow::slotAddEnd()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){
        tableModel->insertRows(tableModel->rowCount(), 1);
    }
}


void MainWindow::slotAddAfter()
{

    int row = ui->rafficherTable->selectionModel()->currentIndex().row();

    if(row >= 0){
        tableModel->insertRows(row+1, 1);
    }
}



void MainWindow::on_rafficherTable_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "Adding context menu";
    QMenu * menu = new QMenu(this);

    QAction * addBefore = new QAction(tr("Ajouter avant"), this);
    QAction * addAfter = new QAction(tr("Ajouter apres"), this);
    QAction * addEnd = new QAction(tr("Ajouter à la fin"), this);
    QAction * deleteRow = new QAction(tr("Supprimer"), this);

    connect(addBefore, SIGNAL(triggered()), this, SLOT(slotAddBefore()));
    connect(addAfter, SIGNAL(triggered()), this, SLOT(slotAddAfter()));
    connect(addEnd, SIGNAL(triggered()), this, SLOT(slotAddEnd()));
    connect(deleteRow, SIGNAL(triggered()), this, SLOT(slotDeleteRow()));

    menu->addAction(addBefore);
    menu->addAction(addAfter);
    menu->addAction(addEnd);
    menu->addAction(deleteRow);

    qDebug() << "done context menu";

    menu->popup(ui->rafficherTable->viewport()->mapToGlobal(pos));
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    tableModel->setFilter("lower(NOM) LIKE lower('%" + arg1 + "%')");
}


void MainWindow::on_toolButton_clicked()
{
    ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_clicked()
{
    QDialog QFileDialog;
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    /*QPrinter printer;*/





    QTextDocument doc;
    QSqlQuery q;
    q.prepare("SELECT * FROM FILEATTENTE");
    q.exec();
    QString pdf="<br> <img src='C:\\Users\\Ahmed2\\Documents\\GitHub\\BonApp\\logo.png' height='42' width='144'/> <b><h1  style='color:blue'>File d'attente :  </b><br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> RECETTE </th> <th> ETAT </th>  </tr>" ;

    while ( q.next()) {
        pdf = pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>   <td>   " + q.value(1).toString() +"</td>   <td>" +q.value(2).toString() +"  "" " "</td>";
    }

    doc.setHtml(pdf);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
    //ui->rafficherTable->render(&printer);
}

void MainWindow::on_pushButton_2_clicked()
{
    queue nqueue = queue(0, ui->lineEdit_2->text(), ui->lineEdit_3->text());

    nqueue.ajouter();

    ui->queueTableView->setModel(Qtmp.afficher());

}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->queueTableView->selectionModel()->hasSelection())
    {


            QModelIndexList selection = ui->queueTableView->selectionModel()->selectedRows();
            qDebug() << "selection count : " << selection.count();


            for(int i=0; i< selection.count(); i++)
            {
                if(i==0)
                {
                    if (QMessageBox::warning(this,
                                             tr("Supprimer lignes"),
                                             tr("Etes vous sur de bien vouloir supprimer les lignes selectionnés définitivement ?"),
                                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
                    {
                        break;
                    }
                }
                QModelIndex index = selection.at(i);
                int id = ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),0)).toString().toInt();
                qDebug() << "deleting queue id : " << id;
                Qtmp.supprimer(id);
            }


    }

    ui->queueTableView->setModel(Qtmp.afficher());

}

void MainWindow::on_pushButton_5_clicked()
{
    if(ui->queueTableView->selectionModel()->hasSelection())
    {

        if (QMessageBox::warning(this,
                                 tr("Supprimer toutes les taches"),
                                 tr("Etes vous sur de bien vouloir supprimer toutes les lignes définitivement ?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            Qtmp.supprimer(-1);
        }

    }

    ui->queueTableView->setModel(Qtmp.afficher());
}

void MainWindow::on_queueTableView_pressed(const QModelIndex &index)
{

}

void MainWindow::on_queueTableView_clicked(const QModelIndex &index)
{
    lastQueue = ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),0)).toString().toInt();
    ui->lineEdit_4->setText(ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),1)).toString());
    ui->lineEdit_5->setText(ui->queueTableView->model()->data(ui->queueTableView->model()->index(index.row(),2)).toString());
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    queue nqueue = queue(lastQueue, ui->lineEdit_4->text(), ui->lineEdit_5->text());

    nqueue.modifier();

    ui->queueTableView->setModel(Qtmp.afficher());
}

void MainWindow::on_returnbtn1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_returnbtn1_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_3_pressed()
{

}
