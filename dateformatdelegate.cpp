#include "dateformatdelegate.h"
#include<QDateTime>

DateFormatDelegate::DateFormatDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QString DateFormatDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    //QVariant valDuree = ui->rafficherTable->model()->data(ui->rafficherTable->model()->index(index.row(),2));




    QString res = QDateTime::fromTime_t(value.toInt()).toUTC().toString("hh:mm:ss");
    return res;
}
