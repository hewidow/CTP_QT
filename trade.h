#ifndef TRADE_H
#define TRADE_H

#include <QDialog>

namespace Ui {
class Trade;
}

class Trade : public QDialog
{
    Q_OBJECT

public:
    explicit Trade(QWidget *parent = nullptr);
    ~Trade();

private:
    Ui::Trade *ui;
};

#endif // TRADE_H
