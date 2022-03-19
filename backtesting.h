#pragma once

#include <QDialog>
#include <QCloseEvent>
#include "ui_backtesting.h"
#include "util.h"
#include "debug.h"

class Backtesting : public QDialog
{
	Q_OBJECT

public:
	Backtesting(QWidget *parent = Q_NULLPTR);
	~Backtesting();
	// �½�����
	void receiveReqOrderInsert(CThostFtdcInputOrderField);
	// �޸ı���
	void receiveReqOrderAction(CThostFtdcInputOrderActionField);
	// ��������
	void loadData();
protected:
	// ��д�ر��¼�
	void closeEvent(QCloseEvent* event) override;
private slots:
	// ��ʼ�ز�
	void on_start_clicked();
	// ��ͣ�ز�
	void on_stop_clicked();
signals:
	// ���ͻز⿪ʼ������ͣ��״̬
	void sendBacktestingStatus(bool);
    // �����˻��ʽ���Ϣ
    void sendTradingAccount(TradingAccount);
    // ����Ͷ���ֲ߳�
    void sendInvestorPositions(QVector<CThostFtdcInvestorPositionField>);
    // ���ͱ�����Ϣ
    void sendOrders(QVector<CThostFtdcOrderField>);
	// ���ĺ�Լ��Ӧ
	void sendRtnDepthMarketData(QuoteField);



private:
	Ui::Backtesting ui;
};
