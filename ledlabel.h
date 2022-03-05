#ifndef LEDLABEL_H
#define LEDLABEL_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class LedLabel : public QWidget
{
public:
    explicit LedLabel(QWidget *parent = nullptr,QString text="");
    void setColor(QString color="red");
    void setText(QString text);
private:
    QLabel *led;
    QLabel *label;
};

#endif // LEDLABEL_H
