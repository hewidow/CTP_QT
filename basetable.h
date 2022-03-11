#ifndef BASETABLE_H
#define BASETABLE_H

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include "util.h"
#include "debug.h"

class BaseTable : public QTableView
{
    Q_OBJECT
public:
    explicit BaseTable(QWidget *parent = nullptr,QList<QString> _headers={});
    void clearData(); // 清空数据
protected:
    QStandardItemModel* model;
    QList<QString>headers;
};

#endif // BASETABLE_H
