#include "ledlabel.h"

LedLabel::LedLabel(QWidget *parent,QString text) : QWidget(parent)
{
    led=new QLabel(this);
    label=new QLabel(this);
    QHBoxLayout *hLayout=new QHBoxLayout(this);
    hLayout->setMargin(0); // 外边距设为0
    hLayout->setSpacing(0); // 子组件间隙设为0
    hLayout->addWidget(led);
    hLayout->addWidget(label);
    setColor("red");
    setText(text);
}
void LedLabel::setColor(QString color)
{
    led->setStyleSheet("min-width: 16px; min-height: 16px;max-width: 16px; max-height: 16px;border-radius: 8px;  border:none;background:"+color+";");
}
void LedLabel::setText(QString text)
{
    label->setText(text);
}
