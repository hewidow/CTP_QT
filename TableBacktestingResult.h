#pragma once

#include "TableBase.h"
#include "global.h"

class TableBacktestingResult : public TableBase
{
	Q_OBJECT

public:
	explicit TableBacktestingResult(QWidget* parent = nullptr);
public slots:
	void receiveBacktestingResult(BacktestingResult);
};
