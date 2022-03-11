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
	// ��Ӳ���
	void on_add_clicked();
	// ɾ������
	void on_del_clicked();
	// �༭����
    void on_edit_clicked();
	// ��������
    void on_start_clicked();
	// ��ͣ����
    void on_pause_clicked();
public:
	StrategyModel strategyModel; // ����ģ��
private:
	Ui::Strategy ui;
	QStandardItemModel *model;
};
