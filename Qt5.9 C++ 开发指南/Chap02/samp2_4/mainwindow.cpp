#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);// 初始化可视化创建的控件
    InitializeUI();// 初始化编码创建的控件
    InitializeSingleAndSolt();// 为编码创建的控件设置槽函数
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化控件
void MainWindow::InitializeUI()
{
    fLabCurFile = new QLabel;// 创建标签
    fLabCurFile->setMinimumWidth(150);// 设置标签的最小宽度
    fLabCurFile->setText("当前文件:");// 设置标签显示文本
    ui->statusBar->addWidget(fLabCurFile);// 添加标签到状态栏

    progressBar1 = new QProgressBar;// 创建进度条
    progressBar1->setMaximumWidth(200);// 设置进度条的最大宽度
    progressBar1->setMaximum(50);// 进度条最大值
    progressBar1->setMinimum(5);// 进度条最小值
    int nPointSize = ui->txtEdit->font().pointSize();// 获取字体的大小
    progressBar1->setValue(nPointSize);// 设置进度条当前值
    ui->statusBar->addWidget(progressBar1);// 添加到状态栏

    spinFontSize = new QSpinBox;// 创建选值框
    spinFontSize->setMinimum(5);// 最小值
    spinFontSize->setMaximum(50);// 最大值
    spinFontSize->setMaximumWidth(50);// 设置选值框的最大宽度
    nPointSize = ui->txtEdit->font().pointSize();// 获取字体的大小
    spinFontSize->setValue(nPointSize);// 设置选值框的值
    ui->mainToolBar->addWidget(new QLabel("字体大小"));// 创建标签并添加到菜单栏
    ui->mainToolBar->addWidget(spinFontSize);// 添加到菜单栏

    ui->mainToolBar->addSeparator();// 添加分隔符

    ui->mainToolBar->addWidget(new QLabel("字体"));// 创建标签并添加到菜单栏
    comboFont = new QFontComboBox;// 创建字体组合框
    comboFont->setMinimumWidth(150);// 组合框最大宽度
    ui->mainToolBar->addWidget(comboFont);// 添加到菜单栏

    setCentralWidget(ui->txtEdit);// 设置中央窗口
}

// 初始化信号&槽
void MainWindow::InitializeSingleAndSolt()
{
    // 为字体大小设置槽响应函数
    connect(spinFontSize, SIGNAL(valueChanged(int)), this, SLOT(on_spinFontSize_valueChanged(int)));
    // 为字体名称设置槽响应函数
    connect(comboFont, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_comboFont_currentIndexChanged(QString)));
}

// 字体大小发生变化
void MainWindow::on_spinFontSize_valueChanged(int value)
{
    QTextCharFormat fmt = ui->txtEdit->currentCharFormat();// 获取txtEdit当前字体
    fmt.setFontPointSize(value);// 设置字体大小
    ui->txtEdit->setCurrentCharFormat(fmt);// 设置txtEdit当前字体
}

// 字体名称发生变化
void MainWindow::on_comboFont_currentIndexChanged(QString value)
{
    QTextCharFormat fmt = ui->txtEdit->currentCharFormat();// 获取txtEdit当前字体
    fmt.setFontFamily(value);// 设置字体名称
    ui->txtEdit->setCurrentCharFormat(fmt);// 设置txtEdit当前字体
}

// 设置粗体
void MainWindow::on_actFontBold_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt = ui->txtEdit->currentCharFormat();
    // 设置粗体
    if(checked)
        fmt.setFontWeight(QFont::Bold);
    else fmt.setFontWeight(QFont::Normal);
    ui->txtEdit->setCurrentCharFormat(fmt);
}

// 设置斜体
void MainWindow::on_actFontItalic_triggered(bool checked)
{
    QTextCharFormat fmt = ui->txtEdit->currentCharFormat();
    fmt.setFontItalic(checked);// 设置斜体
    ui->txtEdit->setCurrentCharFormat(fmt);
}

// 设置下划线
void MainWindow::on_actFontUnder_triggered(bool checked)
{
    QTextCharFormat fmt = ui->txtEdit->currentCharFormat();
    fmt.setFontUnderline(checked);// 设置下划线
    ui->txtEdit->setCurrentCharFormat(fmt);
}

// 有内容可以被复制时发射
void MainWindow::on_txtEdit_copyAvailable(bool b)
{
    ui->actCopy->setEnabled(b);// 设置Copy按钮是否启用
    ui->actCut->setEnabled(b);// 设置Cut按钮是否启用
    ui->actClear->setEnabled(ui->txtEdit->canPaste());// 设置Clear是否启用
}

// 选择的文字发生变化时发射
void MainWindow::on_txtEdit_selectionChanged()
{
    QTextCharFormat fmt = ui->txtEdit->currentCharFormat();
    ui->actFontItalic->setChecked(fmt.fontItalic());// 设置Italic是否被选中
    ui->actFontBold->setChecked(fmt.font().bold());// 设置Bold是否被选中
    ui->actFontUnder->setChecked(fmt.fontUnderline());// 设置Under是否被选中
}
