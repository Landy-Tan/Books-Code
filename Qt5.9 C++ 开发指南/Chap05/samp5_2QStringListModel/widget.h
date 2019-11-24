#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QStringListModel *m_pStrListModel;
};

#endif // WIDGET_H
