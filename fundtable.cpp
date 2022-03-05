#include "fundtable.h"

FundTable::FundTable(QWidget *parent) : BaseTable(parent,{"可用资金","冻结资金","可取资金"})
{
    for (int i=0;i<headers.size();++i) {
        model->setItem(0,i,new QStandardItem("正在获取"));
        model->item(0,i)->setTextAlignment(Qt::AlignCenter);
    }
}

void FundTable::receiveTradingAccount(TradingAccount t)
{
    model->setData(model->index(0,0),Util::convertNumberToQString(t.Available,2));
    model->setData(model->index(0,1),Util::convertNumberToQString(t.FrozenCash,2));
    model->setData(model->index(0,2),Util::convertNumberToQString(t.WithdrawQuota,2));
}
