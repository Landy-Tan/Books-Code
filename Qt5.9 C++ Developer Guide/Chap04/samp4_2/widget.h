#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void InitializeSingleAndSlot();

private:
    Ui::Widget *ui;

private slots:
    void on_QPushButton_clicked();
};

#endif // WIDGET_H
