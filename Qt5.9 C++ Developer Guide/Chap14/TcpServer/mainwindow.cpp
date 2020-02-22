#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * 名  称：MainWindow::MainWindow(QWidget *parent)
 * 功  能：构造函数
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_labClientCount.setText("客户端数量:");
    m_labClientCount.setMinimumWidth(200);
    m_labServerState.setText("服务器状态:");
    m_labServerState,setMinimumWidth(200);
    ui->statusBar->addWidget(&m_labServerState);
    ui->statusBar->addWidget(&m_labClientCount);

    ui->spinBox_Port->setMinimum(0);
    ui->spinBox_Port->setMaximum(0xffff);
    ui->spinBox_Port->setValue(7000);

    ui->lineEdit_Address->setText(localIp());
    setWindowTitle(windowTitle() + " ----本机IP地址:" + localIp());
    setCentralWidget(ui->splitter);

    connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(onTcpServerNewConnected()));
}

/*
 * 名  称：MainWindow::~MainWindow()
 * 功  能：析构函数
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 名  称：QString MainWindow::localIp()const
 * 功  能：获取本地IP地址
 * 返回值：成功返回本地IP地址， 否则返回空
 */
QString MainWindow::localIp()const
{
    QString strHostInfo = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(strHostInfo);
    QList<QHostAddress> lsAddr = hostInfo.addresses();
    foreach(QHostAddress addr, lsAddr)
    {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)return addr.toString();
    }
    return QString();
}

/*
 * 名  称：void MainWindow::on_actStartServer_triggered()
 * 功  能：启动服务器
 */
void MainWindow::on_actStartServer_triggered()
{

    if(!m_tcpServer.listen(QHostAddress(ui->lineEdit_Address->text()), quint16(ui->spinBox_Port->value())))
    {
        m_labServerState.setText("服务器状态:启动失败");
        return;
    }
    else m_labServerState.setText("服务器状态:运行中...");
}

/*
 * 名  称：void MainWindow::on_actStopServer_triggered()
 * 功  能：停止服务器
 */
void MainWindow::on_actStopServer_triggered()
{
   if(m_tcpServer.isListening())m_tcpServer.close();
   m_labServerState.setText("服务器状态:停止");
}

/*
 * 名  称：void MainWindow::on_pushButton_SendMsg_clicked()
 * 功  能：发送数据
 */
void MainWindow::on_pushButton_SendMsg_clicked()
{
    QByteArray byWrite = ui->plainTextEdit_SendMsg->toPlainText().toUtf8();
    foreach(QTcpSocket* tcp, m_tcplsClient)
    {
        if(tcp->write(byWrite) != byWrite.size())
        {
            ui->plainText_Msg->appendPlainText("Send[" + tcp->peerAddress().toString() + "]:失败");
        }
    }
}

/*
 * 名  称：void MainWindow::onTcpServerNewConnected()
 * 功  能：接收到客户端连接请求
 */
void MainWindow::onTcpServerNewConnected()
{
    QTcpSocket* tcp = m_tcpServer.nextPendingConnection();
    m_tcplsClient.push_back(tcp);
    connect(tcp, SIGNAL(connected()), this, SLOT(onTcpClientConnected()));
    connect(tcp, SIGNAL(disconnected()), this, SLOT(onTcpClientDisconnected()));
    connect(tcp, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onTcpClientStateChanged(QAbstractSocket::SocketState)));
    connect(tcp, SIGNAL(readyRead()), this, SLOT(onTcpClientReadyRead()));
    ui->plainText_Msg->appendPlainText("**客户端:" + tcp->peerAddress().toString() + " 已连接");
    m_labClientCount.setText("客户端数量:" + QString::number((m_tcplsClient.size())));
}

/*
 * 名  称：void MainWindow::onTcpClientConnected()
 * 功  能：客户端连接
 */
void MainWindow::onTcpClientConnected()
{

    m_labClientCount.setText("客户端数量:" + QString::number((m_tcplsClient.size())));
}

/*
 * 名  称：void MainWindow::onTcpClientDisconnected()
 * 功  能：客户端断开连接
 */
void MainWindow::onTcpClientDisconnected()
{
    QTcpSocket* tcp = static_cast<QTcpSocket*>(sender());
    ui->plainText_Msg->appendPlainText("**客户端:" + tcp->peerAddress().toString() + " 断开连接");
    m_tcplsClient.removeOne(tcp);
    tcp->abort();
    m_labClientCount.setText("客户端数量:" + QString::number((m_tcplsClient.size())));
}

/*
 * 名  称：void MainWindow::onTcpClientStateChanged(QAbstractSocket::SocketState socketState)
 * 功  能：更新客户端状态
 */
void MainWindow::onTcpClientStateChanged(QAbstractSocket::SocketState socketState)
{

}

/*
 * 名  称：void MainWindow::onTcpClientReadyRead()
 * 功  能：客户端有数据可读
 */
void MainWindow::onTcpClientReadyRead()
{
    QTcpSocket* tcp = static_cast<QTcpSocket*>(sender());
    QByteArray byRead = tcp->readAll();
    ui->plainText_Msg->appendPlainText("Recv[" + tcp->peerAddress().toString() + "]:" + QString(byRead));
}

