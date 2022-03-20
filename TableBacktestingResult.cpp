#include "TableBacktestingResult.h"

TableBacktestingResult::TableBacktestingResult(QWidget* parent) : TableBase(parent, { "名称","结果" })
{
	horizontalHeader()->setVisible(false);
}

void TableBacktestingResult::receiveBacktestingResult(BacktestingResult bResult)
{
    clearData();
	QList<QString> key{
		"起始交易日",
		"结束交易日",
		"总交易日",
		"盈利交易日",
		"亏损交易日",
		"起始资金",
		"结束资金",
		"总收益率",
		"年化收益",
		"最大回撤",
		"百分比最大回撤",
		"总盈亏",
		"总手续费",
		"总成交额",
		"总成交笔数"
	};
	QList<QString>value{
		bResult.startTime.toString(),
		bResult.endTime.toString(),
		Util::convertNumberToQString(bResult.totalTradingDays),
		Util::convertNumberToQString(bResult.profitableTradingDays),
		Util::convertNumberToQString(bResult.lossTradingDays),
		Util::convertNumberToQString(bResult.startFund),
		Util::convertNumberToQString(bResult.endFund),
		Util::convertNumberToQString(bResult.profitRate),
		Util::convertNumberToQString(bResult.annualizedProfit),
		Util::convertNumberToQString(bResult.maximumDrawdown),
		Util::convertNumberToQString(bResult.maximumDrawdownPercent),
		Util::convertNumberToQString(bResult.totalProfitAndLoss),
		Util::convertNumberToQString(bResult.totalHandlingFee),
		Util::convertNumberToQString(bResult.totalTurnover),
		Util::convertNumberToQString(bResult.totalTransactionNum)
	};
    for (int i = 0; i < key.size(); ++i) {
        model->setItem(i, 0, new QStandardItem(key[i]));
        model->setItem(i, 1, new QStandardItem(value[i]));
        for (int j = 0; j < headers.size(); ++j) {
            model->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}