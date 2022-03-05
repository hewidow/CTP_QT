#ifndef BASETABLE_H
#define BASETABLE_H

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include "global.h"
#include "util.h"

class BaseTable : public QTableView
{
public:
    explicit BaseTable(QWidget *parent = nullptr,QList<QString> _headers={});
protected:
    QStandardItemModel* model;
    QList<QString>headers;
};

#endif // BASETABLE_H
