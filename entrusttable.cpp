#include "entrusttable.h"

EntrustTable::EntrustTable(QWidget *parent) : BaseTable(parent,{"报单编号","代码","买卖方向","价格","总数量","剩余数量","状态","时间"})
{

}
QList<QString> EntrustTable::formatData(Order o)
{
    return {
        o.OrderSysID,
        o.InstrumentID,
        Util::convertDirectionToText(o.Direction),
        Util::convertNumberToQString(o.LimitPrice),
        Util::convertNumberToQString(o.VolumeTotalOriginal),
        Util::convertNumberToQString(o.VolumeTotal),
        Util::convertOrderStatusToText(o.OrderStatus),
        o.InsertTime
    };
}
void EntrustTable::receiveOrder(Order t)
{
    QList<QString>tp=formatData(t);
    if (idToRow.count(t.OrderSysID)==0) {
        idToRow[t.OrderSysID]=model->rowCount();
        int index=model->rowCount();
        for (int i=0;i<headers.size();++i) {
            model->setItem(index,i,new QStandardItem(tp[i]));
            model->item(index,i)->setTextAlignment(Qt::AlignCenter);
        }
    } else {
        int index=idToRow[t.OrderSysID];
        for (int i=0;i<headers.size();++i) {
            model->setData(model->index(index,i),tp[i]);
        }
    }
}

