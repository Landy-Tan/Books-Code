#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btnToBinary, SIGNAL(clicked()), this, SLOT(on_btnTo_clicked()));
    connect(ui->btnToDecimalism, SIGNAL(clicked()), this, SLOT(on_btnTo_clicked()));
    connect(ui->btnToHexadecimal, SIGNAL(clicked()), this, SLOT(on_btnTo_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnCalc_clicked()
{
    int nNumber = ui->editNum->text().toInt();// 数量
    float flUnit = ui->editUnitPrice->text().toFloat();// 单价

    // 以下两行等价
    //ui->editTotalPrice->setText(QString::asprintf("%.2f", nNumber * flUnit));
    ui->editTotalPrice->setText(QString::number(flUnit * nNumber, 'f', 2));
}

void Widget::on_btnTo_clicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn == ui->btnToBinary)
    {
        ui->editBinary->setText(QString::number(ui->editBinary->text().toInt(), 2).toUpper());
    }else if(btn == ui->btnToDecimalism)
    {
        ui->editDecimalism->setText((QString::number(ui->editDecimalism->text().toInt(), 10).toUpper()));
    }else if(btn == ui->btnToHexadecimal)
    {
        ui->editHexadecimal->setText((QString::number(ui->editHexadecimal->text().toInt(), 16).toUpper()));
    }
}
