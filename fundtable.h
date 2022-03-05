#ifndef FUNDTABLE_H
#define FUNDTABLE_H

#include "basetable.h"

class FundTable : public BaseTable
{

public:
    explicit FundTable(QWidget *parent = nullptr);
public slots:
    void receiveTradingAccount(TradingAccount);
};

#endif // FUNDTABLE_H
