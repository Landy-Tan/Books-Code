#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // 创建节点时用作type参数
    enum treeItemType{itTopItem=1001,itGroupItem, itImageItem};
    // 目录树的编号
    enum treeColNum{colItem=0, colItemType=1};

private:
    QLabel *LabFileName;    // 状态栏文件名显示
    QPixmap *curPixmap;      // 当前的图片
    float pixRation;        // 当前图片缩放比例

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actAddFolder_triggered();

    void on_actAddFiles_triggered();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actZoomFitH_triggered();

    void on_actDeleteItem_triggered();

    void on_actScanItems_triggered();

    void on_actZoomFitW_triggered();

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actZoomRealSize_triggered();

    void on_actDockVisible_triggered(bool checked);

    void on_actDockFloat_triggered(bool checked);

    void on_dockWidget_visibilityChanged(bool visible);

    void on_dockWidget_topLevelChanged(bool topLevel);

private:
    Ui::MainWindow *ui;

private:
    void InitializeTree();// 初始化Tree
    void AddFolderItem(QTreeWidgetItem *parItem, QString dirName);  // 添加目录
    QString GetFinalFolderName(const QString &fullPathName);        // 提取目录名称
    void AddImageItem(QTreeWidgetItem *parItem, QString aFileName); // 添加图片
    void DisplayImage(QTreeWidgetItem *parItem);                    // 显示一个图片节点的图片
    void ChangeItemCaption(QTreeWidgetItem *parItem);               // 遍历改变节点标题
};

#endif // MAINWINDOW_H
