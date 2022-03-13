#include "strategy.h"

Strategy::Strategy(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    ui.pause->setVisible(false);

    QDir dir;
    if (!dir.exists(STRATEGY_PATH)) dir.mkpath(STRATEGY_PATH);

    dir.setPath(STRATEGY_PATH);
    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.cpp";
    dir.setNameFilters(filters);
    QStringList names = dir.entryList();

    model= new QStandardItemModel(this);
    for (auto &name : names) {
        model->appendRow(new QStandardItem(name));
    }
    ui.listView->setModel(model); // 显示STRATEGY_PATH路径下所有后缀为cpp的文件名
    ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    connect(ui.listView, &QListView::doubleClicked, this, &Strategy::on_edit_clicked); // 绑定双击事件
}

Strategy::~Strategy()
{

}


void Strategy::on_add_clicked()
{
    iDebug << "add";
}
void Strategy::on_del_clicked()
{
    iDebug << "del";
}

void Strategy::on_edit_clicked()
{
    QString path = STRATEGY_PATH + ui.listView->currentIndex().data().toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absoluteFilePath()));
}
void Strategy::on_start_clicked()
{
    strategyModel.start();
    ui.start->setVisible(false);
    ui.pause->setVisible(true);
}

void Strategy::on_pause_clicked()
{
    strategyModel.pause();
    ui.start->setVisible(true);
    ui.pause->setVisible(false);
}
