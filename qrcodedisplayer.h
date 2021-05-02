#ifndef QRCODEDISPLAYER_H
#define QRCODEDISPLAYER_H

#include <QDialog>
#include <QPainter>
namespace Ui {
class qrcodedisplayer;
}

class qrcodedisplayer : public QDialog
{
    Q_OBJECT

public:
    explicit qrcodedisplayer(QWidget *parent = nullptr);
    ~qrcodedisplayer();
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::qrcodedisplayer *ui;
};

#endif // QRCODEDISPLAYER_H
