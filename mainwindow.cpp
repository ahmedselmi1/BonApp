#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materiel.h"
#include "produit.h"
#include<QMessageBox>
#include<math.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView_2->setModel(Mtmp.afficher());
    ui->tableView->setModel(Ptmp.afficher(ui->lineEdit_13->text(), ui->comboBox->currentIndex()));
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_2_clicked()
{
  ui->stackedWidget ->setCurrentIndex(4);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget ->setCurrentIndex(2);
}

void MainWindow::on_pushButton_4_clicked()
{
      ui->stackedWidget ->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget ->setCurrentIndex(1);
}

void MainWindow::on_pushButton_5_clicked()
{
     ui->stackedWidget ->setCurrentIndex(3);
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget ->setCurrentIndex(2);
}

void MainWindow::on_pushButton_12_clicked()
{
   ui->stackedWidget ->setCurrentIndex(4);
}

void MainWindow::on_pushButton_8_clicked()
{
   ui->stackedWidget ->setCurrentIndex(5);
}

void MainWindow::on_pushButton_13_clicked()
{
  int id_materiel=ui->lineEdit_3 ->text().toInt();
int nb_materiel=ui->lineEdit_5 ->text().toInt();
QString nom_materiel=ui->lineEdit_4 ->text();
QString date_entr=ui->lineEdit_6 ->text();
materiel M( nb_materiel,id_materiel,nom_materiel,date_entr);
bool test=M.ajouter();
        if(test)
{  ui->tableView_2->setModel(Mtmp.afficher());
            QMessageBox::information(nullptr,QObject::tr("ok"),
                           QObject::tr("ajout effectué\n"
                                       "click Cancel to exit"), QMessageBox::Cancel);

        }
        else
            QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("Ajout non effectué\n"
                                                                            "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButton_9_clicked()
{
     ui->stackedWidget ->setCurrentIndex(6);
}

void MainWindow::on_pushButton_16_clicked()
{
      ui->stackedWidget ->setCurrentIndex(4);
}

void MainWindow::on_pushButton_15_clicked()
{
   int id_materiel =ui->lineEdit_11 ->text().toInt();
   bool test=Mtmp.supprimer(id_materiel);
   if(test)
   {
        ui->tableView_2->setModel(Mtmp.afficher());
     QMessageBox::information(nullptr,QObject::tr("OK"),
             QObject::tr("Suppression effectué.\n"
                         "click Cancel to exit"), QMessageBox::Cancel);
   }
   QMessageBox::information(nullptr,QObject::tr("NOT OK"),
           QObject::tr("Suppression non effectué.\n"
                       "click Cancel to exit"),QMessageBox::Cancel);
}

void MainWindow::on_pushButton_14_clicked()
{
    int id_ingrediant=ui->lineEdit_7 ->text().toInt();
  int quantite_ingredient=ui->lineEdit_10 ->text().toInt();
  QString nom_ingredient=ui->lineEdit_8 ->text();
  QString date_exp=ui->lineEdit_9 ->text();
  produit P( quantite_ingredient,id_ingrediant,nom_ingredient,date_exp);
  bool test=P.ajouter();
          if(test)
  {      ui->tableView->setModel(Ptmp.afficher(ui->lineEdit_13->text(), ui->comboBox->currentIndex()));

              QMessageBox::information(nullptr,QObject::tr("ok"),
                             QObject::tr("ajout effectué\n"
                                         "click Cancel to exit"), QMessageBox::Cancel);

          }
          else
              QMessageBox::critical(nullptr,QObject::tr("Not OK"),QObject::tr("Ajout non effectué\n"
                                                                              "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget ->setCurrentIndex(0);

}


void MainWindow::on_pushButton_17_clicked()
{
     ui->stackedWidget ->setCurrentIndex(2);
}


void MainWindow::on_pushButton_18_clicked()
{
    int id_ingrediant =ui->lineEdit_12->text().toInt();
    bool test=Ptmp.supprimer(id_ingrediant);
    if(test)
    {
        ui->tableView->setModel(Ptmp.afficher(ui->lineEdit_13->text(), ui->comboBox->currentIndex()));

      QMessageBox::information(nullptr,QObject::tr("OK"),
              QObject::tr("Suppression effectué.\n"
                          "click Cancel to exit"), QMessageBox::Cancel);
    }
    QMessageBox::information(nullptr,QObject::tr("NOT OK"),
            QObject::tr("Suppression non effectué.\n"
                        "click Cancel to exit"),QMessageBox::Cancel);
}

void MainWindow::on_pushButton_6_clicked()
{
    if (ui->pushButton_6->isChecked())
               {
                  // ui->pushButton_modifier->setDisabled(true);
                  // ui->pushButton_6->setText("Terminer Modif");
                   QSqlTableModel *tableModel= new QSqlTableModel();
                   tableModel->setTable("PRODUIT");
tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

           //tableModel->setFilter("where lower(NOM_INGREDIENT) like lower('%" + ui->lineEdit_13->text() + "%')");
                   tableModel->select();
                   ui->tableView->setModel(tableModel);
               }
               else
               {
                   //ui->pushButton_6->setText("Modifier");
                   ui->tableView->setModel(Ptmp.afficher(ui->lineEdit_13->text(), ui->comboBox->currentIndex()));

               }
}

QString MainWindow::getSearchText()
{
    return ui->lineEdit_13->text();
}

void MainWindow::on_lineEdit_13_textChanged(const QString &arg1)
{
    ui->tableView->setModel(Ptmp.afficher(ui->lineEdit_13->text(), ui->comboBox->currentIndex()));
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->tableView->setModel(Ptmp.afficher(ui->lineEdit_13->text(), ui->comboBox->currentIndex()));
}



void MainWindow::on_PDF_clicked()
{
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
            QString pdf="<br> <img src='C:\\Users\\Karim\\Desktop\\QT\\BonApp\\img\\logo.png' height='42' width='144'/> <h1  style='color:#d88855'>       LISTE DES PRODUITS  <br></h1>\n <br> <table>  <tr>  <th> ID </th> <th> quantite </th> <th> nom </th>  <th> date_exp  </th>   </tr>" ;


            while ( q.next()) {

                pdf= pdf+ " <br> <tr> <td>"+ q.value(0).toString()+"    </td>  <td>   " + q.value(1).toString() +"</td>  <td>" +q.value(2).toString() +"  "" " "</td>      <td>     "+q.value(3).toString()+"--"+"</td>       <td>"+q.value(4).toString()+"             </td>" ;

            }
            doc.setHtml(pdf);
            doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
            doc.print(&printer);
}

void MainWindow::on_pushButton_19_clicked()
{
    produit p;
    ui->label_stat_1->setNum(((p.stati())*100/p.nb_total()));
    ui->label_stat2->setNum(((p.stati1())*100/p.nb_total()));
    ui->label_12->setText("%");
    ui->label_13->setText("%");
    ui->label_stat_3->setNum(((p.stati2())*100/p.nb_total()));





}
