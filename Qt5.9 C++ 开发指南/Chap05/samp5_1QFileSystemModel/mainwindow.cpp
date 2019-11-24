#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter_3);
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());

    ui->treeView->setModel(model);
    ui->listView->setModel(model);
    ui->tableView->setModel(model);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), ui->listView, SLOT(setRootIndex(QModelIndex)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), ui->tableView, SLOT(setRootIndex(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->chkIsDir->setChecked(model->isDir(index));
    ui->LabPath->setText(model->filePath(index));
    ui->LabType->setText(model->type(index));
    ui->LabFileName->setText(model->fileName(index));
    int nFileSize = model->size(index)/1024;
    if(nFileSize < 1024)ui->LabFileSize->setText(QString("%1 KB").arg(nFileSize));
    else ui->LabFileSize->setText(QString::asprintf("%.1f MB", nFileSize / 1024.0));
}
