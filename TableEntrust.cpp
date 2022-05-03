#include "TableEntrust.h"

TableEntrust::TableEntrust(QWidget* parent) : TableBase(parent, { "编号","代码","方向","开平","价格","总数量","剩余数量","状态","时间" })
{
	horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents); // 设置第6列根据内容调整宽度
	horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents); // 设置第6列根据内容调整宽度
}
QList<QString> TableEntrust::formatData(CThostFtdcOrderField o)
{
	return {
		QString(o.OrderSysID).trimmed(), // 去除开头空白字符
		o.InstrumentID,
		Util::convertDirectionToText(o.Direction),
		Util::convertCombOffsetFlagToText(o.CombOffsetFlag),
		Util::convertNumberToQString(o.LimitPrice),
		Util::convertNumberToQString(o.VolumeTotalOriginal),
		Util::convertNumberToQString(o.VolumeTotal),
		Util::convertOrderStatusToText(o.OrderStatus),
		o.InsertTime
	};
}
void TableEntrust::receiveOrders(QVector<CThostFtdcOrderField> orders)
{
	clearData();
	std::sort(orders.begin(), orders.end(), [](const CThostFtdcOrderField& x, const CThostFtdcOrderField& y) {
		return x.BrokerOrderSeq < y.BrokerOrderSeq;
		});
	for (int i = 0; i < orders.size(); ++i) {
		QList<QString>tp = formatData(orders[i]);
		for (int j = 0; j < headers.size(); ++j) {
			model->setItem(i, j, new QStandardItem(tp[j]));
			model->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
}

