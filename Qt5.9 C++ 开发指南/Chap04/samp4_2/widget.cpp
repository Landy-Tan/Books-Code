#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitializeSingleAndSlot();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitializeSingleAndSlot()
{
    QList<QPushButton*> Buttons;
    Buttons.push_back(ui->btnAppend);
    Buttons.push_back(ui->btnContains);
    Buttons.push_back(ui->btnCount);
    Buttons.push_back(ui->btnEndsWith);
    Buttons.push_back(ui->btnIndexOf);
    Buttons.push_back(ui->btnIsEmpty);
    Buttons.push_back(ui->btnIsNull);
    Buttons.push_back(ui->btnLastIndexOf);
    Buttons.push_back(ui->btnLeft);
    Buttons.push_back(ui->btnPrepend);
    Buttons.push_back(ui->btnRight);
    Buttons.push_back(ui->btnSection);
    Buttons.push_back(ui->btnSimplified);
    Buttons.push_back(ui->btnSize);
    Buttons.push_back(ui->btnStartsWith);
    Buttons.push_back(ui->btnToLower);
    Buttons.push_back(ui->btnToUpper);
    Buttons.push_back(ui->btnTrimmed);

    foreach(QPushButton * Button, Buttons)
    {
        connect(Button, SIGNAL(clicked()), this, SLOT(on_QPushButton_clicked()));
    }
}

void Widget::on_QPushButton_clicked()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    QString szResult;
    if(Button == ui->btnAppend){
        szResult = ui->editStr1->text().append(ui->editStr2->text());
    }else if(Button == ui->btnPrepend){
        szResult = ui->editStr1->text().prepend(ui->editStr2->text());
    }else if(Button == ui->btnToUpper){
        szResult = ui->editResult->text().toUpper();
    }else if(Button == ui->btnToLower){
        szResult = ui->editResult->text().toLower();
    }else if(Button == ui->btnCount){
        szResult = ui->editStr1->text().count();
    }else if(Button == ui->btnSize){
        szResult = ui->editStr1->text().size();
    }else if(Button == ui->btnTrimmed){
        szResult = ui->editStr1->text().trimmed();
    }else if(Button == ui->btnSimplified){
        szResult = ui->editStr1->text().simplified();
    }else if(Button == ui->btnIndexOf){
        szResult = QString::number(ui->editStr1->text().indexOf(ui->editStr2->text()), 10);
    }else if(Button == ui->btnLastIndexOf){
        szResult = QString::number(ui->editStr1->text().lastIndexOf(ui->editStr2->text()), 10);
    }else if(Button == ui->btnIsNull){
        szResult = ui->editStr1->text().isNull()?"True":"False";
    }else if(Button == ui->btnIsEmpty){
        szResult = ui->editStr1->text().isEmpty()?"True":"False";
    }else if(Button == ui->btnContains){
        szResult = ui->editStr1->text().contains(ui->editStr2->text())?"True":"False";
    }else if(Button == ui->btnEndsWith){
        szResult = ui->editStr1->text().endsWith(ui->editStr2->text())?"True":"False";
    }else if(Button == ui->btnStartsWith){
        szResult = ui->editStr1->text().startsWith(ui->editStr2->text())?"True":"False";
    }else if(Button == ui->btnLeft){
        szResult = ui->editStr1->text().left(1);
    }else if(Button == ui->btnRight){
        szResult = ui->editStr1->text().right(1);
    }else if(Button == ui->btnSection){
        szResult = ui->editStr1->text().section(",",0,1);
    }











    ui->editResult->setText(szResult);


}
