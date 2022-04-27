#include "TableBacktestResult.h"

TableBacktestResult::TableBacktestResult(QWidget* parent) : TableBase(parent, { "名称","结果" })
{
	horizontalHeader()->setVisible(false);
}

void TableBacktestResult::receiveBacktestResult(BacktestResult bResult)
{
	clearData();
	QList<QString> key{
		"开始交易日",
		"结束交易日",
		"总交易次数",
		"盈利交易次数",
		"亏损交易次数",
		"起始资金",
		"结束资金",
		"总收益率",
		"年化收益率",
		"最大回撤率",
		"总盈亏",
		"总手续费"
	};
	QList<QString>value{
		bResult.startTime.toString("yyyy/MM/dd"),
		bResult.endTime.toString("yyyy/MM/dd"),
		Util::convertNumberToQString(bResult.totalTransactions),
		Util::convertNumberToQString(bResult.profitTransactions),
		Util::convertNumberToQString(bResult.lossTransactions),
		Util::convertNumberToQString(bResult.startFund,2),
		Util::convertNumberToQString(bResult.endFund,2),
		Util::convertNumberToQString(bResult.profitRate * 100,2) + "%",
		Util::convertNumberToQString(bResult.annualizedProfitRate * 100,2) + "%",
		Util::convertNumberToQString(bResult.maximumDrawdownRate * 100,2) + "%",
		Util::convertNumberToQString(bResult.totalProfitAndLoss,2),
		Util::convertNumberToQString(bResult.totalHandlingFee,2)
	};
	for (int i = 0; i < key.size(); ++i) {
		model->setItem(i, 0, new QStandardItem(key[i]));
		model->setItem(i, 1, new QStandardItem(value[i]));
		for (int j = 0; j < headers.size(); ++j) {
			model->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
}