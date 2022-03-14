#pragma once

#include <QDialog>
#include "ui_backtesting.h"

class Backtesting : public QDialog
{
	Q_OBJECT

public:
	Backtesting(QWidget *parent = Q_NULLPTR);
	~Backtesting();

private:
	Ui::Backtesting ui;
};
