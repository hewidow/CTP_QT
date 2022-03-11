#ifndef ENTRUSTTABLE_H
#define ENTRUSTTABLE_H

#include "basetable.h"

class EntrustTable : public BaseTable
{
    Q_OBJECT
public:
    explicit EntrustTable(QWidget *parent = nullptr);
    QList<QString> formatData(CThostFtdcOrderField);
public slots:
    void receiveOrders(QVector<CThostFtdcOrderField>); // 接收报单信息
};

#endif // ENTRUSTTABLE_H
