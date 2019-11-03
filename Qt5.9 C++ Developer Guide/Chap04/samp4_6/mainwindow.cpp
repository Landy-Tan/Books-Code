#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitializeWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::InitializeWindow()
{
    // 居中
    setCentralWidget(ui->splitter);

    // 图标
    QIcon iconSelect;
    iconSelect.addFile(":/images/全部项目.png");

    // 下拉式菜单
    QMenu *menuSelectItem = new QMenu();
    menuSelectItem->addAction(ui->actSelAll);
    menuSelectItem->addAction(ui->actSelNone);
    menuSelectItem->addAction(ui->actSelInvs);

    // 创建工具栏上的下拉式菜单按钮
    QToolButton *tBtnSelectItem = new QToolButton;
    tBtnSelectItem->setMenu(menuSelectItem);
    tBtnSelectItem->setPopupMode(QToolButton::InstantPopup);
    tBtnSelectItem->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tBtnSelectItem->setDefaultAction(ui->actSelPopMenu);
    ui->mainToolBar->addWidget(tBtnSelectItem);

    // 菜单栏上的分隔条
    ui->mainToolBar->addSeparator();

    // 工具栏上的退出按钮
    /* 方法1
    QToolButton *tBtnExit = new QToolButton;
    tBtnExit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tBtnExit->setDefaultAction(ui->actQuit);
    ui->mainToolBar->addWidget(tBtnExit);*/

    /* 方法2 */
    ui->mainToolBar->addAction(ui->actQuit);

    // 主窗口的下拉式菜单按钮
    ui->tBtnSelectItem->setMenu(menuSelectItem);
    ui->tBtnSelectItem->setPopupMode(QToolButton::MenuButtonPopup);
    ui->tBtnSelectItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->tBtnSelectItem->setDefaultAction(ui->actSelPopMenu);


    connect(ui->actListAppend, SIGNAL(triggered()), this, SLOT(on_actListInsert_triggered()));
    connect(ui->actSelAll, SIGNAL(triggered()), this, SLOT(on_actListSelect_triggered()));
    connect(ui->actSelNone, SIGNAL(triggered()), this, SLOT(on_actListSelect_triggered()));
    connect(ui->actSelInvs, SIGNAL(triggered()), this, SLOT(on_actListSelect_triggered()));

    ui->tBtnListAppend->setDefaultAction(ui->actListAppend);
    ui->tBtnListClear->setDefaultAction(ui->actListClear);
    ui->tBtnListDelete->setDefaultAction(ui->actListDelete);
    ui->tBtnListIni->setDefaultAction(ui->actListIni);
    ui->tBtnListInsert->setDefaultAction(ui->actListInsert);
    ui->tBtnListAppend->setMinimumWidth(80);
    ui->tBtnListClear->setMinimumWidth(80);
    ui->tBtnListDelete->setMinimumWidth(80);
    ui->tBtnListIni->setMinimumWidth(80);
    ui->tBtnListInsert->setMinimumWidth(80);

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_listWidget_customContextMenuRequested(QPoint)));

    return true;
}

void MainWindow::on_actListIni_triggered()
{
    QListWidgetItem *aItem;
    QIcon aIcon;
    //aIcon.addFile(":/images/");
    bool chk = ui->checkBox->isChecked();
    ui->listWidget->clear();
    for(int i = 0; i < 10; i++)
    {
        QString str = QString::asprintf("Item %d", i);
        aItem = new QListWidgetItem;
        aItem->setText(str);
        aItem->setIcon(aIcon);
        aItem->setCheckState(Qt::Checked);
        if(chk)
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        else
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        ui->listWidget->addItem(aItem);
    }

}

void MainWindow::on_actListInsert_triggered()
{
    QIcon aIcon;
    QListWidgetItem *aItem = new QListWidgetItem(aIcon, "New Inserted Item");
    aItem->setCheckState(Qt::Checked);
    if(ui->checkBox->isChecked())
        aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    else aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

    QAction* Send = qobject_cast<QAction*>(sender());
    if(Send == ui->actListInsert)
        ui->listWidget->insertItem(ui->listWidget->currentRow(), aItem);
    else if(Send == ui->actListAppend)
        ui->listWidget->addItem(aItem);
}

void MainWindow::on_actListDelete_triggered()
{
    delete ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void MainWindow::on_actListSelect_triggered()
{
    QAction* send = qobject_cast<QAction*>(sender());
    int nCount = ui->listWidget->count();
    for(int i = 0; i < nCount; i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(send == ui->actSelAll)item->setCheckState(Qt::Checked);
        else if(send == ui->actSelNone)item->setCheckState(Qt::Unchecked);
        else if(send == ui->actSelInvs)item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked:Qt::Checked);
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString str;
    if(current)
        str = "当前项:" + current->text();
    if(previous)
        str += "前一项" + previous->text();
    ui->lineEdit->setText(str);

}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actListIni);
    menu->addAction(ui->actListClear);
    menu->addAction(ui->actListAppend);
    menu->addAction(ui->actListInsert);
    menu->addAction(ui->actListDelete);
    menu->addSeparator();
    menu->addAction(ui->actSelAll);
    menu->addAction(ui->actSelNone);
    menu->addAction(ui->actSelInvs);
    menu->exec(QCursor::pos());
    delete menu;
}
