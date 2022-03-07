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
    explicit Trade(QWidget *parent = nullptr);
    ~Trade();
    void showDialog(CThostFtdcRspUserLoginField);
private slots:
    void on_pushButton_clicked();
signals:
    void sendReqOrderInsert(CThostFtdcInputOrderField);

private:
    Ui::Trade *ui;
    CThostFtdcRspUserLoginField userInfo;
};

#endif // TRADE_H
