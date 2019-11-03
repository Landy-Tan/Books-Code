#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LabFileName = new QLabel;
    curPixmap = new QPixmap;
    ui->statusBar->addWidget(LabFileName);
    this->setCentralWidget(ui->scrollArea);
    InitializeTree();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化QTreeWidget组件
void MainWindow::InitializeTree()
{
    QString dataStr="";
    ui->treeWidget->clear();
    QIcon icon;
    icon.addFile(":/Image/新建文件.png");

    // 构建顶层节点
    QTreeWidgetItem *item = new QTreeWidgetItem(MainWindow::itTopItem);
    item->setIcon(MainWindow::colItem, icon);
    item->setText(MainWindow::colItem, "图片文件");
    item->setText(MainWindow::colItemType, "type=itTopItem");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
                   Qt::ItemIsAutoTristate);
    item->setCheckState(colItem, Qt::Checked);// 数据项可勾选

    item->setData(MainWindow::colItem, Qt::UserRole, QVariant(dataStr));
    ui->treeWidget->addTopLevelItem(item);// 添加顶层节点
}

// 添加目录
// 添加一个组节点
void MainWindow::AddFolderItem(QTreeWidgetItem *parItem, QString dirName)
{
    QIcon icon("./Images/放大.png");
    QString NodeText = GetFinalFolderName(dirName);

    QTreeWidgetItem *item = new QTreeWidgetItem(MainWindow::itGroupItem);
    item->setIcon(colItem, icon);
    item->setText(colItem, NodeText);
    //item->setText(colItemType, "type=itGroupItem");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                   Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(colItem, Qt::Checked);
    item->setData(colItem, Qt::UserRole, QVariant(dirName));
    parItem->addChild(item);
}

// 提取目录名称
// 从一个完整目录名称里，获得最后文件夹名称
QString MainWindow::GetFinalFolderName(const QString &fullPathName)
{
    int cnt = fullPathName.length();
    int i = fullPathName.lastIndexOf("/");
    QString str = fullPathName.right(cnt - i - 1);
    return str;
}

void MainWindow::AddImageItem(QTreeWidgetItem *parItem, QString aFileName)
{
    QIcon icon(":/Images/放大.png");
    QString NodeText=GetFinalFolderName(aFileName);

    QTreeWidgetItem *item = new QTreeWidgetItem(MainWindow::itImageItem);
    item->setIcon(MainWindow::colItem, icon);
    item->setText(MainWindow::colItem, NodeText);
    //item->setText(MainWindow::colItemType, "type=itImageItem");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                   Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(MainWindow::colItem, Qt::Checked);
    item->setData(MainWindow::colItem, Qt::UserRole, QVariant(aFileName));
    parItem->addChild(item);
}

void MainWindow::DisplayImage(QTreeWidgetItem *parItem)
{
    try{
        if(parItem == NULL)return;
        QString fileName = parItem->data(colItem, Qt::UserRole).toString();
        LabFileName->setText(fileName);
        QImage image;
        image.load(fileName);
        *curPixmap = QPixmap::fromImage(image);
        on_actZoomFitH_triggered();
    }catch(...)
    {
        perror("QPixmap::load");
    }
}

void MainWindow::ChangeItemCaption(QTreeWidgetItem *parItem)
{
    QString str = "*" + parItem->text(colItem);
    parItem->setText(colItem, str);
    for(int i = 0; parItem->childCount(); i++)
    {
        ChangeItemCaption(parItem->child(i));
    }
}

// 添加目录
// 添加一个组节点
void MainWindow::on_actAddFolder_triggered()
{
    QString dir = QFileDialog::getExistingDirectory();
    if(!dir.isEmpty())
    {
        QTreeWidgetItem *parItem = ui->treeWidget->currentItem();
        AddFolderItem(parItem, dir);
    }
}

// 添加文件
// 添加图片
void MainWindow::on_actAddFiles_triggered()
{
    QStringList dirs = QFileDialog::getOpenFileNames(this, "选择一个或多个文件", "","Image(*.jpg)");
    if(dirs.isEmpty())return;
    QTreeWidgetItem *parItem, *item;
    item = ui->treeWidget->currentItem();
    if(item->type() == itGroupItem)
        parItem = item->parent();
    else parItem = item;

    for(int i = 0; i < dirs.size(); i++)
    {
        QString aFileName = dirs.at(i);
        AddImageItem(parItem, aFileName);
    }

}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if(current == NULL)return;
    int var = current->type();
    switch(var)
    {
    case itTopItem:
        ui->actAddFolder->setEnabled(true);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(false);
        break;
    case itGroupItem:
        ui->actAddFolder->setEnabled(true);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(true);
        break;
    case itImageItem:
        ui->actAddFolder->setEnabled(false);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(true);
        DisplayImage(current);
        break;
    }

}

void MainWindow::on_actDeleteItem_triggered()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    QTreeWidgetItem* parItem = item->parent();
    parItem->removeChild(item);
    delete item;
}

void MainWindow::on_actScanItems_triggered()
{
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        ChangeItemCaption(item);
    }
}


void MainWindow::on_actZoomFitH_triggered()
{
    int H = ui->scrollArea->height();
    int realH = curPixmap->height();
    pixRation = float(H)/realH;
    QPixmap pix = curPixmap->scaledToHeight(H-30);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actZoomFitW_triggered()
{
    int W = ui->scrollArea->width();
    int realW = curPixmap->width();
    pixRation = float(W)/realW;
    QPixmap pix = curPixmap->scaledToWidth(W-30);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actZoomIn_triggered()
{
    pixRation *= 1.2;
    int w = pixRation * curPixmap->width();
    int h = pixRation * curPixmap->height();
    QPixmap pix = curPixmap->scaled(w,h);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actZoomOut_triggered()
{
    pixRation *= 0.8;
    int w = pixRation * curPixmap->width();
    int h = pixRation * curPixmap->height();
    QPixmap pix = curPixmap->scaled(w,h);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actZoomRealSize_triggered()
{
    pixRation = 1;
    ui->label->setPixmap(*curPixmap);
}

void MainWindow::on_actDockVisible_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}

void MainWindow::on_actDockFloat_triggered(bool checked)
{
    ui->dockWidget->setFloating(checked);
}

void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{
    ui->actDockVisible->setChecked(visible);
}

void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{
    ui->actDockFloat->setChecked(topLevel);
}
