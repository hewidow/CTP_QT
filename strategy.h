#pragma once

#include <QWidget>
#include <QDir>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QUrl>
#include "ui_strategy.h"
#include "global.h"
#include "debug.h"
#include "strategymodel.h"

class Strategy : public QWidget
{
	Q_OBJECT
public:
	Strategy(QWidget *parent = Q_NULLPTR);
	~Strategy();
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
public:
	StrategyModel strategyModel; // 策略模型
private:
	Ui::Strategy ui;
	QStandardItemModel *model;
};
