#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materiel.h"
#include "produit.h"
#include<QMessageBox>
#include<math.h>
#include<QSound>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView22->setModel(Mtmp.afficher());
    ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
     ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushbuttonmat_clicked()
{
  ui->stackedWidget ->setCurrentIndex(4);
}

void MainWindow::on_pushbuttonprod_clicked()
{
    ui->stackedWidget ->setCurrentIndex(2);
    QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
}

void MainWindow::on_pushbuttonback2_clicked()
{
      ui->stackedWidget ->setCurrentIndex(1);
}

void MainWindow::on_pushbuttonback1_clicked()
{
    ui->stackedWidget ->setCurrentIndex(1);
}

void MainWindow::on_pushButtonaj1_clicked()
{
     ui->stackedWidget ->setCurrentIndex(3);
      QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
}

void MainWindow::on_pushButtonback_clicked()
{
    ui->stackedWidget ->setCurrentIndex(2);
}

void MainWindow::on_pushButtonback3_clicked()
{
   ui->stackedWidget ->setCurrentIndex(4);
}

void MainWindow::on_pushButtonaj2_clicked()
{
   ui->stackedWidget ->setCurrentIndex(5);
}

void MainWindow::on_pushButtonval2_clicked()
{
  int id_materiel=ui->lineEditmat1 ->text().toInt();
int nb_materiel=ui->lineEditmat4 ->text().toInt();
QString nom_materiel=ui->lineEditmat2 ->text();
QString date_entr=ui->lineEditmat3 ->text();
materiel M( nb_materiel,id_materiel,nom_materiel,date_entr);
bool test=M.ajouter();
        if(test)
{  ui->tableView22->setModel(Mtmp.afficher());
            QMessageBox::information(nullptr,QObject::tr("ok"),
                           QObject::tr("ajout effectué\n"
                                       "click Cancel to exit"), QMessageBox::Cancel);

        }
        else
            QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("Ajout non effectué\n"
                                                                            "Click Cancel to exit."), QMessageBox::Cancel);
}


void MainWindow::on_pushButtonsup2_clicked()
{
    int id_materiel =ui->lineEdit_suppmat ->text().toInt();
    bool test=Mtmp.supprimer(id_materiel);
    if(test)
    {
         ui->tableView22->setModel(Mtmp.afficher());
      QMessageBox::information(nullptr,QObject::tr("OK"),
              QObject::tr("Suppression effectué.\n"
                          "click Cancel to exit"), QMessageBox::Cancel);
    }
    QMessageBox::information(nullptr,QObject::tr("NOT OK"),
            QObject::tr("Suppression non effectué.\n"
                        "click Cancel to exit"),QMessageBox::Cancel);

}



void MainWindow::on_pushButtonvalid1_clicked()
{
    int id_ingrediant=ui->lineEditprod1 ->text().toInt();
  int quantite_ingredient=ui->lineEditprod4 ->text().toInt();
  QString nom_ingredient=ui->lineEditprod2 ->text();
  QString date_exp=ui->lineEditprod3 ->text();
  produit P( quantite_ingredient,id_ingrediant,nom_ingredient,date_exp);
  bool test=P.ajouter();
          if(test)
  {      ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));

              QMessageBox::information(nullptr,QObject::tr("ok"),
                             QObject::tr("ajout effectué\n"
                                         "click Cancel to exit"), QMessageBox::Cancel);

          }
          else
              QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("Ajout non effectué\n"
                                                                              "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButtonsup1_clicked()
{
    //ui->stackedWidget ->setCurrentIndex(0);
    int id_produit =ui->lineEditsup1 ->text().toInt();
    bool test=Ptmp.supprimer(id_produit);
    if(test)
    {
         ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
     N.notifications_supprimerproduit();
    }
    QMessageBox::information(nullptr,QObject::tr("NOT OK"),
            QObject::tr("Suppression non effectué.\n"
                        "click Cancel to exit"),QMessageBox::Cancel);

}




void MainWindow::on_pushButtonmod1_clicked()
{
    if (ui->pushButtonmod1->isChecked())
               {
                  // ui->pushButton_modifier->setDisabled(true);
                  // ui->pushButton_6->setText("Terminer Modif");
                   QSqlTableModel *tableModel= new QSqlTableModel();
                   tableModel->setTable("PRODUIT");
tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

           //tableModel->setFilter("where lower(NOM_INGREDIENT) like lower('%" + ui->lineEdit_13->text() + "%')");
                   tableModel->select();
                   ui->tableView11->setModel(tableModel);
               }
               else
               {
                   //ui->pushButton_6->setText("Modifier");
                   ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));

               }
}

QString MainWindow::getSearchText()
{
    return ui->lineEditrech1->text();
}

void MainWindow::on_lineEditrech1_textChanged(const QString &arg1)
{
    ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
}

void MainWindow::on_comboboxtri1_currentIndexChanged(int index)
{
    ui->tableView11->setModel(Ptmp.afficher(ui->lineEditrech1->text(), ui->comboboxtri1->currentIndex()));
}



void MainWindow::on_PDF11_clicked()
{
     QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
            if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append("liste des produits.pdf"); }

            QPrinter printer(QPrinter::PrinterResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPaperSize(QPrinter::A4);
            printer.setOutputFileName(fileName);

            QTextDocument doc;
            QSqlQuery q;
            q.prepare("SELECT * FROM PRODUIT ");
            q.exec();
            QString pdf="<br> <img src='C:\\Users\\Karim\\Desktop\\QT\\BonApp\\img\\logo.png' height='42' width='144'/> <h1  style='color:#d88855'>       LISTE DES PRODUITS  <br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> nom </th> <th> date_exp </th>  <th> quantité  </th>   </tr>" ;


            while ( q.next()) {

                pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>  <td>   " + q.value(1).toString() +"</td>  <td>" +q.value(2).toString() +"  "" " "</td>      <td>     "+q.value(3).toString()+"--"+"</td>       <td>"+q.value(4).toString()+"             </td>" ;

            }
            doc.setHtml(pdf);
            doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
            doc.print(&printer);

}

void MainWindow::on_stat11_clicked()
{
    sec = new SecDialog(this);
    sec->show();
}
void MainWindow::on_tableView11_activated(const QModelIndex &index)
{
    int totest = ui->tableView11->model()->data(index).toInt();
    QString s=QString::number(totest);
    ui->lineEditsup1->setText(s);
}

void MainWindow::on_pushButtonmod2_clicked()
{
    if (ui->pushButtonmod2->isChecked())
               {
                  // ui->pushButton_modifier->setDisabled(true);
                  // ui->pushButton_10->setText("Terminer Modif");
                   QSqlTableModel *tableModel= new QSqlTableModel();
                   tableModel->setTable("MATERIEL");
tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

           //tableModel->setFilter("where lower(NOM_INGREDIENT) like lower('%" + ui->lineEdit_13->text() + "%')");
                   tableModel->select();
                   ui->tableView22->setModel(tableModel);
               }
               else
               {
                   //ui->pushButton_10->setText("Modifier");
                   ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri1->currentIndex()));

               }
}

void MainWindow::on_PDF_mat_clicked()
{
    QSound::play(":/../../Desktop/QT/ffqf/crunch.wav");
   QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
           if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append("liste des materiels.pdf"); }

           QPrinter printer(QPrinter::PrinterResolution);
           printer.setOutputFormat(QPrinter::PdfFormat);
           printer.setPaperSize(QPrinter::A4);
           printer.setOutputFileName(fileName);

           QTextDocument doc;
           QSqlQuery q;
           q.prepare("SELECT * FROM MATERIEL ");
           q.exec();
           QString pdf="<br> <img src='C:\\Users\\Karim\\Desktop\\QT\\BonApp\\img\\logo.png' height='42' width='144'/> <h1  style='color:#d88855'>       LISTE DES PRODUITS  <br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> nom </th> <th> date_exp </th>  <th> quantité  </th>   </tr>" ;


           while ( q.next()) {

               pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>  <td>   " + q.value(1).toString() +"</td>  <td>" +q.value(2).toString() +"  "" " "</td>      <td>     "+q.value(3).toString()+"--"+"</td>       <td>"+q.value(4).toString()+"             </td>" ;

           }
           doc.setHtml(pdf);
           doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
           doc.print(&printer);

}

void MainWindow::on_pushButton_mat_clicked()
{
    sec = new SecDialog(this);
    sec->show();
}



void MainWindow::on_lineEdit_rech_mat_textChanged(const QString &arg1)
{
     ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri2->currentIndex()));
}

void MainWindow::on_comboboxtri2_currentIndexChanged(int index)
{
    ui->tableView22->setModel(Ptmp.afficher(ui->lineEdit_rech_mat->text(), ui->comboboxtri2->currentIndex()));
}
