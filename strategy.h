#pragma once

#include <QWidget>
#include <QDir>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QUrl>
#include "ui_strategy.h"
#include "global.h"
#include "debug.h"

class Strategy : public QWidget
{
	Q_OBJECT

public:
	Strategy(QWidget *parent = Q_NULLPTR);
	~Strategy();
	void doubleClicked(QModelIndex);

private:
	Ui::Strategy ui;
	QStandardItemModel *model;
};
