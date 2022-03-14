#ifndef TableFund_H
#define TableFund_H

#include "TableBase.h"

class TableFund : public TableBase
{
    Q_OBJECT
public:
    explicit TableFund(QWidget *parent = nullptr);
public slots:
    void receiveTradingAccount(TradingAccount);
};

#endif // TableFund_H
