#ifndef QRWIDGET_H
#define QRWIDGET_H

#include <QWidget>
#include <QTextBrowser>
class textbrowser : public QTextBrowser{
    Q_OBJECT
public:
    explicit textbrowser(QWidget *parent = 0);
};

#endif // QRWIDGET_H
