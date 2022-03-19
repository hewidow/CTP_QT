#pragma once

#include <QDialog>
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
#include "backtesting.h"

class Strategy : public QDialog
{
	Q_OBJECT
public:
	Strategy(QWidget *parent = Q_NULLPTR);
	~Strategy();
	// 检查是否选中一项策略
	bool checkCurrentSelectIndex();
	// 检查参数是否正确及筛选信号
	bool checkEnvironment();
public slots:
	void receiveTradingAccount(TradingAccount);
	void receiveInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
	void receiveOrders(QVector<CThostFtdcOrderField>);
	void receiveRtnDepthMarketData(QuoteField);
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
	void receiveBacktestingStatus(bool);
signals:
	// 发送当前策略状态到主界面状态栏
	void sendStrategyStatus(QString);
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
	// 回测
	void on_backtesting_clicked();
private:
	Ui::Strategy ui;
	QStandardItemModel *model;
	QVector<StrategyBase*>strategies;
	int cur = -1; // 当前正在运行的策略
	// 是否打开回测
	bool strategyBacktesting = false;
	// 策略是否运行
	bool strategyRunning = false;
	Backtesting backtesting;
};
