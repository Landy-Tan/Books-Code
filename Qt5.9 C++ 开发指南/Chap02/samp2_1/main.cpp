#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // 定义并创建应用程序
    QApplication a(argc, argv);
    // 定义并创建窗口
    Widget w;
    // 显示窗口
    w.show();

    // 应用程序运行
    return a.exec();
}
