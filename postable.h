#ifndef POSTABLE_H
#define POSTABLE_H

#include "basetable.h"

class PosTable : public BaseTable
{
public:
    explicit PosTable(QWidget *parent = nullptr);
public slots:
    void receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
};

#endif // POSTABLE_H
