#ifndef POSTABLE_H
#define POSTABLE_H

#include "basetable.h"

class PosTable : public BaseTable
{
    Q_OBJECT
public:
    explicit PosTable(QWidget *parent = nullptr);
public slots:
    void receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
};

#endif // POSTABLE_H
