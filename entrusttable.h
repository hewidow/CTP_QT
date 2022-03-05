#ifndef ENTRUSTTABLE_H
#define ENTRUSTTABLE_H

#include "basetable.h"

class EntrustTable : public BaseTable
{

public:
    explicit EntrustTable(QWidget *parent = nullptr);
    QList<QString> formatData(Order);
public slots:
    void receiveOrder(Order);
private:
    QMap<QString,int>idToRow; // 映射
};

#endif // ENTRUSTTABLE_H
