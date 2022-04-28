#pragma once

#include "TableBase.h"

class TableBacktestResultPos : public TableBase
{
	Q_OBJECT
public:
	explicit TableBacktestResultPos(QWidget* parent = nullptr);
public slots:
	void receiveBacktestResultPos(QMap<QString, BacktestResultPos>);
};
