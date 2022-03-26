#pragma once

#include "TableBase.h"
#include "global.h"

class TableBacktestResult : public TableBase
{
	Q_OBJECT

public:
	explicit TableBacktestResult(QWidget* parent = nullptr);
public slots:
	void receiveBacktestResult(BacktestResult);
};
