#ifndef TableQuote_H
#define TableQuote_H

#include "TableBase.h"

class TableQuote : public TableBase
{
    Q_OBJECT
public:
    explicit TableQuote(QWidget *parent = nullptr);
    QList<QString>formatData(int,QuoteField);
    void updateQuote(QuoteField); // 更新行情
    int findRowIndex(QString); // 返回合约id对应的行数
public slots:
    void receiveRtnDepthMarketData(QuoteField); // 行情数据
private:
    QMap<QString,int>instrumentIdToRowIndex;
};

#endif // TableQuote_H
