#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_pStrListModel = new QStringListModel(this);
    QStringList strList;
    strList << "1" << "2" << "3" << "4";
    m_pStrListModel->setStringList(strList);
    ui->listView->setModel(m_pStrListModel);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

Widget::~Widget()
{
    delete ui;
    delete m_pStrListModel;
}

// 恢复列表
void Widget::on_pushButton_clicked()
{
    QStringList strList;
    strList << "1" << "2" << "3" << "4";
    m_pStrListModel->setStringList(strList);
}

// 添加项
void Widget::on_pushButton_2_clicked()
{
    m_pStrListModel->insertRow(m_pStrListModel->rowCount());
    QModelIndex modelIndex = m_pStrListModel->index(m_pStrListModel->rowCount() - 1);
    m_pStrListModel->setData(modelIndex, "New item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(modelIndex);

}

// 插入项
void Widget::on_pushButton_5_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    m_pStrListModel->insertRow(index.row());
    m_pStrListModel->setData(index, "Inserted item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

// 删除当前项
void Widget::on_pushButton_3_clicked()
{
    QModelIndex removeIndex = ui->listView->currentIndex();
    m_pStrListModel->removeRow(removeIndex.row());
}

// 清除列表
void Widget::on_pushButton_4_clicked()
{
    m_pStrListModel->removeRows(0, m_pStrListModel->rowCount());
}

// 清空文本
void Widget::on_pushButton_7_clicked()
{
    ui->plainTextEdit->clear();
}

// 显示数据模型到StringList
void Widget::on_pushButton_6_clicked()
{
    QStringList strList = m_pStrListModel->stringList();
    foreach(QString sz, strList)
    {
        ui->plainTextEdit->appendPlainText(sz);
    }
}

// QListView被单击 槽函数
void Widget::on_listView_clicked(const QModelIndex &index)
{
    ui->label->setText(QString::asprintf("当前项: Row = %d, Column = %d",index.row(), index.column()));
}
