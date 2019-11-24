#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter);
    m_pModel = new QStandardItemModel(this);
    m_pSelectionModel = new QItemSelectionModel(m_pModel);
    ui->tableView->setModel(m_pModel);
    ui->tableView->setSelectionModel(m_pSelectionModel);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete m_pModel;
    //delete m_pSelectionModel;
}

void MainWindow::on_actOpenFile_triggered()
{
    m_pModel->clear();
    QString szOpenFileName = QFileDialog::getOpenFileName(this, "打开文件", ".", "csv文件(*.csv)");
    if(szOpenFileName.isEmpty())return;
    QFile file(szOpenFileName);
    if(!file.open(QFile::ReadOnly))return;
    char szFileLine[256]{0};
    QString szLine;

    // 设置列标题
    file.readLine(szFileLine, 256);
    szLine = QString::fromLocal8Bit(szFileLine).replace("\r\n", "");// 字符转换，否则会出现乱码
    m_pModel->setHorizontalHeaderLabels(szLine.split(","));

    // 设置内容
    while(!file.atEnd())
    {
        memset(szFileLine, 0, 256);
        if(file.readLine(szFileLine, 256) <= 0)break;
        szLine = QString::fromLocal8Bit(szFileLine).replace("\r\n","");// 字符转换，否则会出现乱码
        QStringList strLs = szLine.split(",");
        QList<QStandardItem*> cols;
        foreach(QString sz, strLs)
        {
            cols.append(new QStandardItem(sz));
        }
        m_pModel->appendRow(cols);
    }

    file.close();
}

void MainWindow::on_actSaveFile_triggered()
{
    QString szSaveFileName = QFileDialog::getSaveFileName(this, "保存文件", ".", "csv文件(*.csv)");
    if(szSaveFileName.isEmpty())return;
    QFile file(szSaveFileName);
    if(!file.open(QFile::Truncate | QFile::WriteOnly | QFile::Text))return;

    const int nRowCount = m_pModel->rowCount();
    const int nColumnCount = m_pModel->columnCount();
    // 标题
    QString strTitle;
    for(int nTitleIndex = 0; nTitleIndex < nColumnCount; nTitleIndex++)
    {
        QStandardItem* item = m_pModel->horizontalHeaderItem(nTitleIndex);
        if(strTitle.length() == 0) strTitle = item->text();
        else strTitle = strTitle + "," + item->text();
    }
    file.write(strTitle.toLocal8Bit() + "\n");

    // 内容
    for(int nRows = 0; nRows < nRowCount; nRows++)
    {
        QString strData;
        for(int nCols = 0; nCols < nColumnCount; nCols++)
        {
            QStandardItem *item = m_pModel->item(nRows, nCols);
            if(strData.length() == 0) strData = item->text();
            else strData = strData + "," + item->text();
        }
        file.write(strData.toLocal8Bit() + "\n");
    }
    file.close();
}

void MainWindow::on_actModelDataLook_triggered()
{
    ui->plainTextEdit->clear();
    const int nRowCount = m_pModel->rowCount();
    const int nColumnCount = m_pModel->columnCount();

    // 标题
    QString strTitle;
    for(int nColumns = 0; nColumns < nColumnCount; nColumns++)
    {
        QStandardItem *item = m_pModel->horizontalHeaderItem(nColumns);
        strTitle = nColumns == 0 ? item->text() : strTitle + "," + item->text();
    }
    ui->plainTextEdit->appendPlainText(strTitle);

    // 内容
    for(int nRows = 0; nRows < nRowCount; nRows++)
    {
        QString strRow;
        for(int nColumns = 0; nColumns < nColumnCount; nColumns++)
        {
            QStandardItem *item = m_pModel->item(nRows, nColumns);
            strRow = nColumns == 0 ? item->text() : strRow + "," + item->text();
        }
        ui->plainTextEdit->appendPlainText(strRow);
    }
}

void MainWindow::on_actAddRow_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QList<QStandardItem*> items;
    items.append(new QStandardItem("Add row, column 1."));
    items.append(new QStandardItem("Add row, column 2."));
    items.append(new QStandardItem("Add row, column 3."));
    m_pModel->insertRow(index.row() + 1, items);
}

void MainWindow::on_actInsertRow_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QList<QStandardItem*> items;
    items.append(new QStandardItem("Insert row, column 1."));
    items.append(new QStandardItem("Insert row, column 2."));
    items.append(new QStandardItem("Insert row, column 3."));
    m_pModel->insertRow(index.row(), items);
}

void MainWindow::on_actDeleteRow_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    m_pModel->removeRow(index.row());
}
