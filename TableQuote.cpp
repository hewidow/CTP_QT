#include "TableQuote.h"

TableQuote::TableQuote(QWidget* parent) : TableBase(parent, { "序号","交易所代码","代码","名称","时间","最新价","涨跌","涨幅%","买一价","买一量","卖一价","卖一量","成交量","开盘价","最高价","最低价" })
{

}
void TableQuote::receiveRtnDepthMarketData(QuoteField q)
{
	updateQuote(q);
}
QList<QString> TableQuote::formatData(int index, QuoteField q)
{
	return {
		QString::number(index + 1),
		q.ExchangeID,
		q.InstrumentID,
		q.InstrumentName,
		q.UpdateTime,
		Util::convertNumberToQString(q.LastPrice),
		Util::convertNumberToQString(q.UpDown),
		Util::convertNumberToQString(q.Gain,2) + (q.Gain==DBL_MAX?"":"%"),
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
void TableQuote::updateQuote(QuoteField q)
{
	int index = findRowIndex(q.InstrumentID);
	QList<QString>tp = formatData(index, q);
	if (index < model->rowCount()) { // 更新数据项
		for (int i = 0; i < headers.size(); ++i) {
			model->setData(model->index(index, i), tp[i]);
		}
	}
	else { // 创建数据项
		for (int i = 0; i < headers.size(); ++i) {
			model->setItem(index, i, new QStandardItem(tp[i]));
			model->item(index, i)->setTextAlignment(Qt::AlignCenter);
		}
	}
	if (q.UpDown < 0) model->item(index, 6)->setForeground(Qt::green);
	else model->item(index, 6)->setForeground(Qt::red);

	if (q.Gain < 0) model->item(index, 7)->setForeground(Qt::green);
	else model->item(index, 7)->setForeground(Qt::red);
}
int TableQuote::findRowIndex(QString instrumentId)
{
	if (instrumentIdToRowIndex.count(instrumentId) == 0) {
		instrumentIdToRowIndex[instrumentId] = model->rowCount();
		return model->rowCount();
	}
	return instrumentIdToRowIndex[instrumentId];
}
