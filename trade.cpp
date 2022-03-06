#include "trade.h"
#include "ui_trade.h"

Trade::Trade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trade)
{
    ui->setupUi(this);
}

Trade::~Trade()
{
    delete ui;
}
