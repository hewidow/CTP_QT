#include "TableFund.h"

TableFund::TableFund(QWidget* parent) : TableBase(parent, { "总额","冻结","可用","持仓盈亏" })
{
	for (int i = 0; i < headers.size(); ++i) {
		model->setItem(0, i, new QStandardItem("正在获取"));
		model->item(0, i)->setTextAlignment(Qt::AlignCenter);
	}
}

void TableFund::receiveTradingAccount(TradingAccount t)
{
	model->setData(model->index(0, 0), Util::convertNumberToQString(t.total, 2));
	model->setData(model->index(0, 1), Util::convertNumberToQString(t.FrozenMargin, 2));
	model->setData(model->index(0, 2), Util::convertNumberToQString(t.Available, 2));
	model->setData(model->index(0, 3), Util::convertNumberToQString(t.PositionProfit, 2));
}
