#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter);
    m_labSocketState.setText("Socket状态");
    ui->statusBar->addWidget(&m_labSocketState);
    ui->spinBox_Port->setMaximum(0xffff);
    ui->spinBox_Port->setValue(7000);
    ui->lineEdit_Address->setText(localIp());
    setWindowTitle(windowTitle() + "----本机IP地址:" + localIp());

    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(onTcpConnected()));
    connect(&m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onTcpDisconnected()));
    connect(&m_tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onTcpStateChanged(QAbstractSocket::SocketState socketState)));
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onTcpReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::localIp()const
{
    QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> lsAddr = hostInfo.addresses();
    foreach(QHostAddress addr, lsAddr)
    {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)return addr.toString();
    }
    return QString();
}

void MainWindow::on_actConnect_triggered()
{
    ui->plainText_Msg->appendPlainText("**正在连接服务器");
    QHostAddress hostAddr(ui->lineEdit_Address->text());
    quint16 nPort = ui->spinBox_Port->value();
    m_tcpSocket.connectToHost(hostAddr, nPort);
}

void MainWindow::on_actDisconnect_triggered()
{
    ui->plainText_Msg->appendPlainText("**关闭与服务器的连接");
    //m_tcpSocket.close();
    m_tcpSocket.disconnectFromHost();
}

void MainWindow::on_pushButton_SendMsg_clicked()
{
    QByteArray byWrite = ui->plainTextEdit_SendMsg->toPlainText().toUtf8();
    if(m_tcpSocket.write(byWrite) != byWrite.size())
    {
        ui->plainText_Msg->appendPlainText("**发送数据失败");
        return;
    }
    ui->plainText_Msg->appendPlainText("Send:" + QString(byWrite));
    ui->plainTextEdit_SendMsg->clear();
}

void MainWindow::onTcpConnected()
{
    ui->plainText_Msg->appendPlainText("**连接服务器完成");
    onTcpStateChanged(m_tcpSocket.state());
}

void MainWindow::onTcpDisconnected()
{
    onTcpStateChanged(m_tcpSocket.state());
}

void MainWindow::onTcpStateChanged(QAbstractSocket::SocketState socketState)
{
    QString strState;
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        strState = "UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        strState = "UnconnectedState";
        break;
    case QAbstractSocket::ConnectingState:
        strState = "ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        strState = "ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        strState = "BoundState";
        break;
    case QAbstractSocket::ClosingState:
        strState = "ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        strState = "ListeningState";
        break;
    default:
        strState = "未知状态";
        break;
    }
    m_labSocketState.setText("Socket状态:" + strState);
}

void MainWindow::onTcpReadyRead()
{
    QByteArray byRead = m_tcpSocket.readAll();
    ui->plainText_Msg->appendPlainText("Recv:" + QString(byRead));
}
