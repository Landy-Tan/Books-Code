#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox->setMinimumWidth(110);
    setCentralWidget(ui->splitter_2);

    // 初始化状态栏
    labCellIndex = new QLabel("当前单元格坐标:", this);
    labCellIndex->setMinimumWidth(250);
    labCellType = new QLabel("当前单元格类型:", this);
    labCellType->setMinimumWidth(200);
    labStudID = new QLabel("学生ID:", this);
    labStudID->setMinimumWidth(200);
    ui->statusBar->addWidget(labCellIndex);
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudID);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 创建一行数据
void MainWindow::CreateItemsARow(int rowNo, QString Name, QString Sex, QDate Birth, QString Nation, bool isPM, int score)
{
    QString str;
    uint StudID = 201605000;// 学号
    // 设置姓名
    QTableWidgetItem *item = new QTableWidgetItem(Name, CellType::ctName);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    StudID += rowNo;
    item->setData(Qt::UserRole, QVariant(StudID));// 设置学号为Data
    ui->tableWidget->setItem(rowNo, FiledColNum::colName, item);

    // 设置性别
    QIcon icon;
    if(Sex == "男")
        icon.addFile(":/Images/男孩.png");
    else icon.addFile(":/Images/女孩.png");
    item = new QTableWidgetItem(Sex, CellType::ctSex);
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo, FiledColNum::colSex, item);

    // 设置出生日期
    str = Birth.toString("yyyy-MM-dd");
    item = new QTableWidgetItem(str, CellType::ctBirth);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo, FiledColNum::colBirth, item);

    // 设置名族
    item = new QTableWidgetItem(Nation, CellType::ctNation);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo, FiledColNum::colNation, item);

    // 设置党员
    item = new QTableWidgetItem("党员", CellType::ctPartyM);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if(isPM)item->setCheckState(Qt::Checked);
    else item->setCheckState(Qt::Unchecked);
    item->setBackgroundColor(Qt::yellow);
    ui->tableWidget->setItem(rowNo, FiledColNum::colPartyM, item);

    // 设置分数
    str.setNum(score);
    item = new QTableWidgetItem(str, CellType::ctScore);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo, FiledColNum::colScore, item);

}

// 设置列标题
void MainWindow::on_pushButton_clicked()
{
    QStringList headerText;
    headerText << "姓    名" << "性   别" << "出生日期" << "名    族" << "分   数" << "是否党员";
    ui->tableWidget->setColumnCount(headerText.count());
    ui->tableWidget->setHorizontalHeaderLabels(headerText);

    /*
    QTableWidgetItem *headerItem;
    for(int i = 0; i < headerText.count(); i++)
    {
        headerItem = new QTableWidgetItem(headerText.at(i));
        QFont font = headerItem->font();
        font.setBold(true);// 设置为粗体
        font.setPointSize(9);// 设置字体大小
        headerItem->setTextColor(Qt::red);// 设置字体颜色
        headerItem->setFont(font);// 设置字体
        ui->tableWidget->setHorizontalHeaderItem(i, headerItem);
    }
    */
}

// 初始化表格数据
void MainWindow::on_pushButton_3_clicked()
{
    QString strName, strSex;
    bool isParty = false;
    QDate birth;
    birth.setDate(1980,4,7);// 初始化日期
    ui->tableWidget->clearContents();// 清除工作区，不清除表头
    int Rows = ui->tableWidget->rowCount();// 数据区行数
    for(int i = 0; i < Rows; i++)
    {
        strName = QString::asprintf("学生%d", i);
        if((i % 2) == 0)strSex = "男";
        else strSex = "女";

        CreateItemsARow(i, strName, strSex, birth, "汉族", isParty, 70);
        birth = birth.addDays(20);// 日期加20天
        isParty = !isParty;
    }
}

// 设置行数
void MainWindow::on_pushButton_2_clicked()
{
    int rows = ui->spinBox->value();
    ui->tableWidget->setRowCount(rows);
}

// 插入行
void MainWindow::on_pushButton_5_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    ui->tableWidget->insertRow(currentRow);
    CreateItemsARow(currentRow, "新学生", "男", QDate::fromString("2000-1-1", "yyyy-M-d"), "满族", false, 70);
}

// 添加行
void MainWindow::on_pushButton_6_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    CreateItemsARow(rowCount, "新学生", "女", QDate::fromString("2000-1-1", "yyyy-M-d"), "满族", false, 70);
}

// 删除当前行
void MainWindow::on_pushButton_4_clicked()
{
    int nCurrentRow = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(nCurrentRow);
}

// 自动调节行高
void MainWindow::on_pushButton_8_clicked()
{
    ui->tableWidget->resizeRowsToContents();
}

// 自动调节行宽
void MainWindow::on_pushButton_9_clicked()
{
    ui->tableWidget->resizeColumnsToContents();
}

// 表格可编辑
void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked)
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    else ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// 显示行表头
void MainWindow::on_checkBox_3_clicked(bool checked)
{
    ui->tableWidget->horizontalHeader()->setVisible(checked);
}

// 显示列表头
void MainWindow::on_checkBox_4_clicked(bool checked)
{
    ui->tableWidget->verticalHeader()->setVisible(checked);
}

// 间隔行底色
void MainWindow::on_checkBox_2_clicked(bool checked)
{
    ui->tableWidget->setAlternatingRowColors(checked);
}

// 行选择
void MainWindow::on_radioButton_clicked(bool checked)
{
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

// 单元格选择
void MainWindow::on_radioButton_2_clicked(bool checked)
{
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
}

// 读取表格内容到文本
void MainWindow::on_pushButton_7_clicked()
{
    QString str;
    QTableWidgetItem *item;
    ui->plainTextEdit->clear();

    // 列表头
    /*for(int nHorizontalIndex = 0; nHorizontalIndex < ui->tableWidget->columnCount() - 1; nHorizontalIndex++)
        str += ui->tableWidget->horizontalHeaderItem(nHorizontalIndex)->text() + "   ";
    ui->plainTextEdit->appendPlainText(str);*/

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        str = QString::asprintf("%d   ", i);
        for(int j = 0; j < ui->tableWidget->columnCount() - 1; j++)
        {
            item = ui->tableWidget->item(i, j);
            str += item->text() + "   ";
        }

        item = ui->tableWidget->item(i, ui->tableWidget->columnCount() - 1);
        if(item->checkState() == Qt::Checked)
            str += "党员";
        else str += "群众";
        ui->plainTextEdit->appendPlainText(str);
    }

}
