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

    model= new QStandardItemModel(this);;
    for (auto &name : names) {
        model->appendRow(new QStandardItem(name));
    }
    ui.listView->setModel(model); // ��ʾSTRATEGY_PATH·�������к�׺Ϊcpp���ļ���
    ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // ��ֹ�༭
    connect(ui.listView, &QListView::doubleClicked, this, &Strategy::on_edit_clicked); // ��˫���¼�
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
