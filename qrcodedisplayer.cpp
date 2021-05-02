#include "qrcodedisplayer.h"
#include "ui_qrcodedisplayer.h"
#include <QPainter>
qrcodedisplayer::qrcodedisplayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qrcodedisplayer)
{
    ui->setupUi(this);
}

qrcodedisplayer::~qrcodedisplayer()
{
    delete ui;
}


void qrcodedisplayer::paintEvent(QPaintEvent *event) {

QPainter painter(this);
painter.drawRect(QRect(80,120,200,100));

}
