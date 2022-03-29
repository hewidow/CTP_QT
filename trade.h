#ifndef TRADE_H
#define TRADE_H

#include <QDialog>
#include "global.h"
#include "debug.h"

namespace Ui {
	class Trade;
}

class Trade : public QDialog
{
	Q_OBJECT

public:
	explicit Trade(QWidget* parent = nullptr);
	~Trade();
private slots:
	void on_add_clicked();
	void on_del_clicked();
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);

private:
	Ui::Trade* ui;
};

#endif // TRADE_H
