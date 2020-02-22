#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QHostInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    QString localIp()const;

private slots:
    void on_actConnect_triggered();
    void on_actDisconnect_triggered();
    void on_pushButton_SendMsg_clicked();

private slots:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpStateChanged(QAbstractSocket::SocketState socketState);
    void onTcpReadyRead();

private:
    Ui::MainWindow *ui;
    QTcpSocket m_tcpSocket;
    QLabel m_labSocketState;
};

#endif // MAINWINDOW_H
