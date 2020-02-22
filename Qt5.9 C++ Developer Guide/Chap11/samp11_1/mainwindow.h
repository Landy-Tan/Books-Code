#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openTable();
    void getFieldNames();

protected slots:
    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

private slots:
    void on_actOpenDB_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase DB;
    QSqlTableModel *tabModel;
    QItemSelectionModel *theSelection;
    QDataWidgetMapper *dataMapper;
};

#endif // MAINWINDOW_H
