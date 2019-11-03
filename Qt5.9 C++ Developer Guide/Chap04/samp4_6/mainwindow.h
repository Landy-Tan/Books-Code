#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool InitializeWindow();
private slots:
    void on_actListIni_triggered();

    void on_actListInsert_triggered();

    void on_actListDelete_triggered();

    void on_actListSelect_triggered();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
