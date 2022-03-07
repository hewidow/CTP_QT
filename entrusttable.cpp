#include "entrusttable.h"

EntrustTable::EntrustTable(QWidget *parent) : BaseTable(parent,{"报单编号","代码","买卖方向","价格","总数量","剩余数量","状态","时间"})
{

}
QList<QString> EntrustTable::formatData(CThostFtdcOrderField o)
{
    return {
        Util::convertNumberToQString(o.BrokerOrderSeq),
        o.InstrumentID,
        Util::convertDirectionToText(o.Direction),
        Util::convertNumberToQString(o.LimitPrice),
        Util::convertNumberToQString(o.VolumeTotalOriginal),
        Util::convertNumberToQString(o.VolumeTotal),
        Util::convertOrderStatusToText(o.OrderStatus),
        o.InsertTime
    };
}

void EntrustTable::receiveOrders(QVector<CThostFtdcOrderField> orders)
{
    clearData();
    std::sort(orders.begin(),orders.end(),[](const CThostFtdcOrderField &x,const CThostFtdcOrderField &y){
        return x.BrokerOrderSeq<y.BrokerOrderSeq;
    });
    for (int i=0;i<orders.size();++i) {
        QList<QString>tp=formatData(orders[i]);
        for (int j=0;j<headers.size();++j) {
            model->setItem(i,j,new QStandardItem(tp[j]));
            model->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

