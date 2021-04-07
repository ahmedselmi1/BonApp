#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recette.h"
#include <QMessageBox>
#include <QDebug>
#include<QFileDialog>
#include<QPrinter>

#include<QTextDocument>




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

    auto header = ui->rafficherTable->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, [this](int logicalIndex){
        QString text = ui->rafficherTable->model()->headerData(logicalIndex, Qt::Horizontal).toString();
        isAscending = !isAscending;
        tableModel->sort(logicalIndex, isAscending?Qt::AscendingOrder:Qt::DescendingOrder);
       qDebug() << logicalIndex << text;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_recettesBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_queueBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_rajouterBtn_clicked()
{
    int id = ui->rajouterIdTxt->text().toInt();
    QString nom = ui->rajouterNomTxt->text();
    int duree = ui->rajouterTime->time().second()+ui->rajouterTime->time().minute()*60+ui->rajouterTime->time().hour()*60*60;

    recette r(id,nom,duree);
    bool test = false;
    if(rmodif) test = r.modifier();
    else test = r.ajouter();

    if(rmodif){

        if(test)
        {
            QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Modification effectué\nClick Cancel to exit."), QMessageBox::Cancel);
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Modification non effectué\nClick Cancel to exit."), QMessageBox::Cancel);
        }
    }
    else
    {
        if(test)
        {
            QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajout effectué\nClick Cancel to exit."), QMessageBox::Cancel);
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Ajout non effectué\nClick Cancel to exit."), QMessageBox::Cancel);
        }
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_recettesTab_currentChanged(int index)
{
    if(index!=0)
    {
        rmodif = false;
    }
    if(index==1){
        //ui->rafficherTable->setModel(Rtmp.afficher());
    }
}

void MainWindow::on_rsupprimerBtn_clicked()
{
    int id = ui->rsupprimerTxt->text().toInt();
    bool test = Rtmp.supprimer(id);

    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Suppression effectué\nClick Cancel to exit."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Suppression non effectué\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_rafficherTable_doubleClicked(const QModelIndex &index)
{
    if(true) return;
    QVariant valId = ui->rafficherTable->model()->data(ui->rafficherTable->model()->index(index.row(),0));
    QVariant valNom = ui->rafficherTable->model()->data(ui->rafficherTable->model()->index(index.row(),1));
    QVariant valDuree = ui->rafficherTable->model()->data(ui->rafficherTable->model()->index(index.row(),2));

    int heures = (valDuree.toInt())/(60*60);

    int minutes = ((valDuree.toInt()-heures*60*60)%60)/60;

    int secondes = ((valDuree.toInt()-heures*60*60-minutes*60));
    ui->rajouterIdTxt->setText(QString::number(valId.toInt()));
    ui->rajouterNomTxt->setText(valNom.toString());
    QTime ctime;

    ctime.setHMS(heures, minutes, secondes);
    ui->rajouterTime->setTime(ctime);
    rmodif = true;
    ui->recettesTab->setCurrentIndex(0);
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
                if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append("reclamation.pdf"); }

                QPrinter printer(QPrinter::PrinterResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setPaperSize(QPrinter::A4);
                printer.setOutputFileName(fileName);

                /*QPrinter printer;*/
                ui->rafficherTable->render(&printer);




                QTextDocument doc;
                QSqlQuery q;
                q.prepare("SELECT * FROM reclamation ");
                q.exec();
                QString pdf="<br> <img src='D:/Esprit/2A2/projet c++/logo/debug2.png' height='42' width='144'/> <h1  style='color:red'>LISTE DES RECLAMATION  <br></h1>\n <br> <table>  <tr>  <th> CODE </th> <th> SUJET </th> <th> COMMENTAIRE </th> <th> DATE AJOUT </th>  </tr>" ;


                while ( q.next()) {

                    pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>   <td>   " + q.value(1).toString() +"</td>   <td>" +q.value(2).toString() +"  "" " "</td>   <td>"+q.value(3).toString()+"</td>    <td>"+q.value(4).toString()+" </td>" ;

                }
                doc.setHtml(pdf);
                doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
             /*   doc.print(&printer);*/
                ui->rafficherTable->render(&printer);
}
