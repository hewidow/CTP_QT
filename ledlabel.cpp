#include "ledlabel.h"

LedLabel::LedLabel(QWidget* parent, QString color, QString text) : QWidget(parent)
{
	led = new QLabel(this);
	label = new QLabel(this);
	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->setMargin(3); // 外边距
	hLayout->setSpacing(3); // 子组件间隙
	hLayout->addWidget(led);
	hLayout->addWidget(label);
	setStatus(color, text);
}
void LedLabel::setStatus(QString color, QString text)
{
	led->setStyleSheet("min-width: 14px; min-height: 14px;max-width: 14px; max-height: 14px;border-radius: 7px;  border:none;background:" + color + ";");
	label->setText(text);
}
