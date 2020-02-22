#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter_3);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    if(DB.isOpen())DB.close();
    delete ui;
}

void MainWindow::openTable()
{
    tabModel = new QSqlTableModel(this, DB);
    tabModel->setTable("employee");
    tabModel->setSort(tabModle->fieldIndex("EmpNo"), Qt::AscendingOrder);
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if(!tabModel->select()){
        QMessageBox::critical(this, "错误", "打开数据表错误,错误信息:\n" + tabModel->lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

}

void MainWindow::getFieldNames()
{

}

void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{

}

void MainWindow::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{

}

void MainWindow::on_actOpenDB_triggered()
{
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setHostName("127.0.0.1");
    DB.setPort(3306);
    DB.setDatabaseName("demodb");
    DB.setUserName("root");
    DB.setPassword("123456");
    if(!DB.open()){
        QMessageBox::warning(this, "错误", "打开数据库失败", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    openTable();
}
