#include "postable.h"

PosTable::PosTable(QWidget *parent) : BaseTable(parent,{"代码","当前持仓","盈亏"})
{

}
void PosTable::receiveInvestorPosition(InvestorPosition t)
{
    if (idToRow.count(t.InstrumentID)==0) {
        idToRow[t.InstrumentID]=model->rowCount();
        int index=model->rowCount();
        model->setItem(index,0,new QStandardItem(t.InstrumentID));
        model->setItem(index,1,new QStandardItem(Util::convertNumberToQString(t.Position)));
        model->setItem(index,2,new QStandardItem(Util::convertNumberToQString(t.PositionProfit,2)));
        for (int i=0;i<headers.size();++i) model->item(index,i)->setTextAlignment(Qt::AlignCenter);
    } else {
        int index=idToRow[t.InstrumentID];
        model->setData(model->index(index,0),t.InstrumentID);
        model->setData(model->index(index,1),Util::convertNumberToQString(t.Position));
        model->setData(model->index(index,2),Util::convertNumberToQString(t.PositionProfit,2));
    }
}
