#include "widget.h"
#include "ui_widget.h"
#include <QMetaProperty>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    boy = new QPerson("王小明");
    boy->setProperty("score", 95);  // 静态属性,已经在类中声明了的属性
    boy->setProperty("age", 10);    // 静态属性,已经在类中声明了的属性
    boy->setProperty("sex", "Boy"); // 这个是动态属性，未在类中声明的
    connect(boy, &QPerson::ageChanged, this, &Widget::on_ageChanged);// 关联QPerson::ageChanged信号到槽Widget::on_ageChanged

    girl = new QPerson("张小丽");
    girl->setProperty("score", 81); // 静态属性,已经在类中声明了的属性
    girl->setProperty("age",20);    // 静态属性,已经在类中声明了的属性
    girl->setProperty("sex","Girl");// 这个是动态属性，未在类中声明的
    connect(girl, &QPerson::ageChanged, this, &Widget::on_ageChanged);// 关联QPerson::ageChanged信号到槽Widget::on_ageChanged

    // QPinBox控件的动态属性
    ui->spinBoy->setProperty("isBoy", true);
    ui->spinGirl->setProperty("isBoy", false);

    // 关联控件信号，两个QPinBox控件的valueChanged信号关联到同一个槽函数
    connect(ui->spinBoy, SIGNAL(valueChanged(int)), this, SLOT(on_spin_valueChanged(int)));
    connect(ui->spinGirl, SIGNAL(valueChanged(int)), this, SLOT(on_spin_valueChanged(int)));
}

Widget::~Widget()
{
    delete ui;
}

// 当boy/girl对象的age对象发生改变时，将会执行此函数
void Widget::on_ageChanged(int value)
{
    Q_UNUSED(value)// 这个宏是告诉编译器：我不使用value这个参数，你不要报错
    QPerson *aPerson = qobject_cast<QPerson *>(sender());// 得到触发此槽函数的对象
    // 得到触发者的属性
    QString name = aPerson->property("name").toString();
    QString sex = aPerson->property("sex").toString();
    int age = aPerson->age();
    //int age = aPerson->property("age").toInt();

    // 显示相关信息到编辑框
    ui->textEdit->append(name+","+sex+QString::asprintf(",年龄=%d", age));
}

// QSpinBox控件的值发生改变时，执行此函数
void Widget::on_spin_valueChanged(int arg1)
{
    Q_UNUSED(arg1)// 这个宏是告诉编译器：我不使用arg1这个参数，你不要报错
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(sender());// 得到触发此槽函数的对象
    // 判断触发者的isBoy属性调用boy/girl对象的age属性
    if(spinBox->property("isBoy").toBool())
        boy->setProperty("age", spinBox->value());
    else girl->setAge(spinBox->value());
}

// 改变boy所属的QSpinBox控件的值
void Widget::on_btnBoyInc_clicked()
{
    ui->spinBoy->setValue(ui->spinBoy->value() + 1);
}

// 改变girl所属的QSpinBox控件的值
void Widget::on_btnGirlInc_clicked()
{
    ui->spinGirl->setValue(ui->spinGirl->value() + 1);
}

void Widget::on_btnClassInfo_clicked()
{
    const QMetaObject *meta = boy->metaObject();// 得到boy对象的元对象实例
    ui->textEdit->clear();
    ui->textEdit->append("==元对象信息==\n");
    ui->textEdit->append(QString("类名称: %1\n").arg(meta->className()));// 得到元对象的类名称
    ui->textEdit->append("Property");
    for(int i = meta->propertyOffset(); i < meta->propertyCount(); i++)
    {
        QMetaProperty prop = meta->property(i);// 得到元对象的属性实例
        const char * propName = prop.name();// 得到属性的名称
        QString propValue = boy->property(propName).toString();// 得到属性的值
        ui->textEdit->append(QString("属性名称=%1, 属性值=%2").arg(propName).arg(propValue));// // 显示相关信息到编辑框
    }
    ui->textEdit->append("\n");
    ui->textEdit->append("ClassInfo");
    for(int i = meta->classInfoOffset(); i < meta->classInfoCount(); i++)
    {
        QMetaClassInfo info = meta->classInfo(i);// 得到元对象的类信息
        ui->textEdit->append(QString("Name=%1, Value=%2").arg(info.name()).arg(info.value()));// 显示相关信息到编辑框
    }
}

void Widget::on_btnClear_clicked()
{
    ui->textEdit->clear();
}
