#include "qrcodewidgettest.h"
#include <QPainter>
qrcodewidgettest::qrcodewidgettest()
{

}


void qrcodewidgettest::paintEvent(QPaintEvent *event) {

QPainter painter(this);
painter.drawRect(QRect(80,120,200,100));

}
