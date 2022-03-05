#ifndef POSTABLE_H
#define POSTABLE_H

#include "basetable.h"

class PosTable : public BaseTable
{
public:
    explicit PosTable(QWidget *parent = nullptr);
public slots:
    void receiveInvestorPosition(InvestorPosition);
private:
    QMap<QString,int>idToRow; // 映射
};

#endif // POSTABLE_H
