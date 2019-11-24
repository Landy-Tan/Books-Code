#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class QStandardItemModel;
class QItemSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actOpenFile_triggered();

    void on_actSaveFile_triggered();

    void on_actModelDataLook_triggered();

    void on_actAddRow_triggered();

    void on_actInsertRow_triggered();

    void on_actDeleteRow_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_pModel;
    QItemSelectionModel *m_pSelectionModel;
};

#endif // MAINWINDOW_H
