#include "TableBacktestResultPos.h"

TableBacktestResultPos::TableBacktestResultPos(QWidget* parent) : TableBase(parent, { "代码","盈亏" })
{

}

void TableBacktestResultPos::receiveBacktestResultPos(QMap<QString, BacktestResultPos> r)
{
	clearData();
	QVector<BacktestResultPos>resultPos;
	for (auto& it : r) {
		resultPos.push_back(it);
	}
	qSort(resultPos.begin(), resultPos.end(), [](const BacktestResultPos& a, const BacktestResultPos& b) {
		return a.profitLoss > b.profitLoss;
	});
	for (int i = 0; i < resultPos.size();++i) {
		model->setItem(i, 0, new QStandardItem(resultPos[i].InstrumentID));
		model->setItem(i, 1, new QStandardItem(Util::convertNumberToQString(resultPos[i].profitLoss, 2)));
		for (int j = 0; j < headers.size(); ++j) {
			model->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
}