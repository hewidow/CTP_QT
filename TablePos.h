#ifndef TablePos_H
#define TablePos_H

#include "TableBase.h"

class TablePos : public TableBase
{
	Q_OBJECT
public:
	explicit TablePos(QWidget* parent = nullptr);
public slots:
	void receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
};

#endif // TablePos_H
