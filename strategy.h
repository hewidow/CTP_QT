#pragma once

#include <QWidget>
#include <QDir>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "ui_strategy.h"
#include "global.h"
#include "debug.h"
#include "StrategyBase.h"
#include "StrategyExample.h"

class Strategy : public QWidget
{
	Q_OBJECT
public:
	Strategy(QWidget *parent = Q_NULLPTR);
	~Strategy();
public slots:
	void receiveTradingAccount(TradingAccount);
	void receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
	void receiveOrders(QVector<CThostFtdcOrderField>);
	void receiveAllInstruments(QVector<InstrumentField>);
	void receiveRtnDepthMarketData(QuoteField);
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
signals:
	void sendReqOrderInsert(CThostFtdcInputOrderField);
	void sendReqOrderAction(CThostFtdcInputOrderActionField);
private slots:
	// 添加策略
	void on_add_clicked();
	// 删除策略
	void on_del_clicked();
	// 编辑策略
    void on_edit_clicked();
	// 启动策略
    void on_start_clicked();
	// 暂停策略
    void on_pause_clicked();
private:
	QVector<StrategyBase*>strategies;
	int cur=-1; // 当前正在运行的策略
private:
	Ui::Strategy ui;
	QStandardItemModel *model;
};
