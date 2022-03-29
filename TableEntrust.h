#ifndef TableEntrust_H
#define TableEntrust_H

#include "TableBase.h"

class TableEntrust : public TableBase
{
	Q_OBJECT
public:
	explicit TableEntrust(QWidget* parent = nullptr);
	QList<QString> formatData(CThostFtdcOrderField);
public slots:
	void receiveOrders(QVector<CThostFtdcOrderField>); // 接收报单信息
};

#endif // TableEntrust_H
