#include "qwdlgmanual.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

void QWDlgManual::initUI()
{
    // 创建QCheckBox并初始化名称及布局
    chkBoxUnder = new QCheckBox(tr("Underline"));
    chkBoxItalic = new QCheckBox(tr("Italic"));
    chkBoxBold = new QCheckBox(tr("Bold"));
    QHBoxLayout *Hlay1 = new QHBoxLayout;
    Hlay1->addWidget(chkBoxUnder);
    Hlay1->addWidget(chkBoxItalic);
    Hlay1->addWidget(chkBoxBold);

    // 创建QRadioButton并初始化名称及布局
    rBtnBlack = new QRadioButton(tr("Black"));
    rBtnBlue = new QRadioButton(tr("Blue"));
    rBtnRed = new QRadioButton(tr("Red"));
    QHBoxLayout *Hlay2 = new QHBoxLayout;
    Hlay2->addWidget(rBtnBlack);
    Hlay2->addWidget(rBtnRed);
    Hlay2->addWidget(rBtnBlue);

    // 创建QPushButton并初始化名称及布局
    btnOK = new QPushButton(tr("确定"));
    btnCancel = new QPushButton(tr("取消"));
    btnClose = new QPushButton(tr("关闭"));
    QHBoxLayout *Hlay3 = new QHBoxLayout;
    Hlay3->addStretch();
    Hlay3->addWidget(btnOK);
    Hlay3->addWidget(btnCancel);
    Hlay3->addStretch();
    Hlay3->addWidget(btnClose);

    // 创建QPlainTextEdit并初始化内容及内容字体大小
    txtEdit = new QPlainTextEdit;
    txtEdit->setPlainText("Hello world\n\nIt is my demo");
    QFont f = txtEdit->font();
    f.setPointSize(20);
    txtEdit->setFont(f);

    // 窗体布局
    QVBoxLayout *VLay = new QVBoxLayout;
    VLay->addLayout(Hlay1);
    VLay->addLayout(Hlay2);
    VLay->addWidget(txtEdit);
    VLay->addLayout(Hlay3);
    setLayout(VLay);

}

void QWDlgManual::initSignalSlots()
{
    // 这里是QRadioButton糟函数
    QObject::connect(rBtnBlue, SIGNAL(clicked()), this, SLOT(setTextFontColor()));
    QObject::connect(rBtnBlack, SIGNAL(clicked()), this, SLOT(setTextFontColor()));
    QObject::connect(rBtnRed, SIGNAL(clicked()), this, SLOT(setTextFontColor()));

    // 这里是QCheckBox糟函数
    QObject::connect(chkBoxUnder, SIGNAL(clicked(bool)), this, SLOT(On_chkBoxUnder(bool)));
    QObject::connect(chkBoxItalic, SIGNAL(clicked(bool)), this, SLOT(On_chkBoxItalic(bool)));
    QObject::connect(chkBoxBold, SIGNAL(clicked(bool)), this, SLOT(On_chkBoxBold(bool)));

    // 这里是QPushButton糟函数
    QObject::connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(btnCancel, SIGNAL(clicked()), this, SLOT(raise()));
    QObject::connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
}

// 设置txtEdit内的字体为下划线
void QWDlgManual::On_chkBoxUnder(bool checked)
{
    QFont font = txtEdit->font();
    font.setUnderline(checked);
    txtEdit->setFont(font);
}

// 设置txtEdit内的字体为斜体
void QWDlgManual::On_chkBoxItalic(bool checked)
{
    QFont f = txtEdit->font();
    f.setItalic(checked);
    txtEdit->setFont(f);
}

// 设置txtEdit内的字体为粗体
void QWDlgManual::On_chkBoxBold(bool checked)
{
    QFont f = txtEdit->font();
    f.setBold(checked);
    txtEdit->setFont(f);
}

// 设置txtEdit内的字体颜色
void QWDlgManual::setTextFontColor()
{
    QPalette pale = txtEdit->palette();
    if(rBtnBlack->isChecked())
        pale.setColor(QPalette::Text, Qt::black);
    else if(rBtnBlue->isChecked())
        pale.setColor(QPalette::Text, Qt::blue);
    else if(rBtnRed->isChecked())
        pale.setColor(QPalette::Text, Qt::red);
    txtEdit->setPalette(pale);
}

QWDlgManual::QWDlgManual(QWidget *parent)
    : QDialog(parent)
{
    initUI();// 初始化界面
    initSignalSlots();// 初始化Signal&slot
    setWindowTitle("Form created manually");// 设置窗口标题
}

QWDlgManual::~QWDlgManual()
{

}
