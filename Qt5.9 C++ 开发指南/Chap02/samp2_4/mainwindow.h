#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QSpinBox>
#include <QFontComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *fLabCurFile;            // 状态栏里显示当前文件的Label
    QProgressBar *progressBar1;     // 状态栏上的进度条
    QSpinBox *spinFontSize;         // 字条大小
    QFontComboBox *comboFont;       // 字体名称

protected:
    void InitializeUI();
    void InitializeSingleAndSolt();

private slots:
    void on_spinFontSize_valueChanged(int value);
    void on_comboFont_currentIndexChanged(QString value);
    void on_actFontBold_triggered(bool checked);
    void on_actFontItalic_triggered(bool checked);
    void on_actFontUnder_triggered(bool checked);
    void on_txtEdit_copyAvailable(bool b);
    void on_txtEdit_selectionChanged();
};

#endif // MAINWINDOW_H
