#include "quotetable.h"

QuoteTable::QuoteTable(QWidget *parent) : BaseTable(parent,{"序号","代码","名称","时间","最新价","涨跌","涨幅%","买一价","买一量","卖一价","卖一量","成交量","开盘价","最高价","最低价"})
{

}
QList<QString> QuoteTable::formatData(int index,QuoteField q)
{
    return {
        QString::number(index+1),
        q.InstrumentID,
        q.InstrumentName,
        q.UpdateTime,
        Util::convertNumberToQString(q.LastPrice),
        Util::convertNumberToQString(q.UpDown),
        Util::convertNumberToQString(q.Gain,2)+"%",
        Util::convertNumberToQString(q.BidPrice1),
        Util::convertNumberToQString(q.BidVolume1),
        Util::convertNumberToQString(q.AskPrice1),
        Util::convertNumberToQString(q.AskVolume1),
        Util::convertNumberToQString(q.Volume),
        Util::convertNumberToQString(q.OpenPrice),
        Util::convertNumberToQString(q.HighestPrice),
        Util::convertNumberToQString(q.LowestPrice)
    };
}
void QuoteTable::updateQuote(QuoteField q)
{
    int index=findRowIndex(q.InstrumentID);
    QList<QString>tp=formatData(index,q);
    if (index<model->rowCount()) { // 更新数据
        for (int i=0;i<headers.size();++i) {
            model->setData(model->index(index,i),tp[i]);
        }
    } else { // 创建项目        
        for (int i=0;i<headers.size();++i) {
            model->setItem(index,i,new QStandardItem(tp[i]));
            model->item(index,i)->setTextAlignment(Qt::AlignCenter);
        }
    }
    if (q.UpDown<0) model->item(index,5)->setForeground(Qt::green);
    else model->item(index,5)->setForeground(Qt::red);

    if (q.Gain<0) model->item(index,6)->setForeground(Qt::green);
    else model->item(index,6)->setForeground(Qt::red);
}
int QuoteTable::findRowIndex(QString instrumentId)
{
    if (instrumentIdToRowIndex.count(instrumentId)==0) {
        instrumentIdToRowIndex[instrumentId]=model->rowCount();
        return model->rowCount();
    }
    return instrumentIdToRowIndex[instrumentId];
}
