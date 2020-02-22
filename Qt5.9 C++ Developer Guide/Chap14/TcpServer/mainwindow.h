#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
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
    void on_actStartServer_triggered();
    void on_actStopServer_triggered();
    void on_pushButton_SendMsg_clicked();

private slots:
    void onTcpServerNewConnected();
    void onTcpClientConnected();
    void onTcpClientDisconnected();
    void onTcpClientStateChanged(QAbstractSocket::SocketState socketState);
    void onTcpClientReadyRead();

private:
    Ui::MainWindow *ui;
    QTcpServer m_tcpServer;
    QList<QTcpSocket*> m_tcplsClient;
    QLabel m_labClientCount;
    QLabel m_labServerState;
};

#endif // MAINWINDOW_H
