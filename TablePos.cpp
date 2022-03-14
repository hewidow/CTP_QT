#include "TablePos.h"

TablePos::TablePos(QWidget *parent) : TableBase(parent,{"代码","当前持仓","盈亏"})
{

}

void TablePos::receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField> positions)
{
    clearData();
    for (int i=0;i<positions.size();++i) {
        model->setItem(i,0,new QStandardItem(positions[i].InstrumentID));
        model->setItem(i,1,new QStandardItem(Util::convertNumberToQString(positions[i].Position)));
        model->setItem(i,2,new QStandardItem(Util::convertNumberToQString(positions[i].PositionProfit,2)));
        for (int j=0;j<headers.size();++j) {
            model->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
