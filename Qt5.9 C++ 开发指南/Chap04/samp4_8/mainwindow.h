#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    // 单元格类型
    enum CellType
    {
        ctName=1000,
        ctSex,
        ctBirth,
        ctNation,
        ctPartyM,
        ctScore
    };
    // 单元格列号
    enum FiledColNum
    {
        colName=0,
        colSex,
        colBirth,
        colNation,
        colScore,
        colPartyM
    };
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_checkBox_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_checkBox_4_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *labCellIndex;
    QLabel *labCellType;
    QLabel *labStudID;
    void CreateItemsARow(int rowNo, QString Name, QString Sex, QDate Birth, QString Nation, bool isPM, int score);
};

#endif // MAINWINDOW_H
