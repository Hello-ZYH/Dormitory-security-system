#include "server.h"
#include "ui_server.h"
#include <windows.h>



Server::Server(QWidget *parent) : QWidget(parent), ui(new Ui::Server)
{
    ui->setupUi(this);
    LoadSetting();
}



Server::~Server()
{
    delete ui;
}



//刷新TCPIP
void Server::RefreshTcpIp()
{
    getLocalHostAddress();
}



//清空接收
void Server::ClearRecv()
{
    on_btn_clear_recv_clicked();
}



//关闭TCP服务
void Server::CloseTcpServer()
{
    tcpServer->close();
}



//配置TCP界面和数据
void Server::LoadSetting()
{
    tcpListenStatus=false;
    //widget
    setWindowTitle(QString("TCP并发服务端"));
    resize(QSize(CURSIZE));
    setMaximumSize(QSize(MAXSIZE));
    setMinimumSize(QSize(MINSIZE));

    //tcp default
    tcpServer = new QTcpServer(this);
    tcpAddress = getLocalHostAddress();
    tcpPort = TCP_PORT;

    //ui
    ui->btn_send->setEnabled(false);
    ui->comboBox_ip->setCurrentText(tcpAddress.toString());
    ui->lineEdit_PORT->setText(QString::number(tcpPort));

    //groupbox
    onlineCount=0;
    onlineTitle=QString("在线列表  当前人数:%1").arg(onlineCount);
    ui->groupBox_online->setTitle(onlineTitle);

    connect(tcpServer,&QTcpServer::newConnection,this,&Server::Process_Client_Connect);
}



//连接到TCP服务器
void Server::ConnectToTcp()
{
    tcpPort=ui->lineEdit_PORT->text().toUInt();
    tcpAddress.setAddress(ui->comboBox_ip->currentText());
    qDebug()<<"state:"<<tcpServer->isListening();
    if(tcpAddress.toString().isEmpty()) return ;
    if(tcpServer->listen(tcpAddress,tcpPort))
    {
        qDebug()<<"state connect:"<<tcpServer->isListening();
        ui->comboBox_ip->setEnabled(false);
        ui->lineEdit_PORT->setEnabled(false);
        ui->btn_send->setEnabled(true);
        QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+"TCP服务启动成功!";
        ui->textBrowser_recv->append(msg);
        setWindowTitle(QString("TCP并发服务端    [%1:%2]").arg(tcpAddress.toString()).arg(tcpPort));
    }
    else
    {
        qDebug()<<"state disconnect:"<<tcpServer->isListening();
        setWindowTitle(QString("TCP并发服务端"));
        QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+tcpServer->errorString();
        ui->textBrowser_recv->append(msg);
    }
}



//断开连接从TCP服务器
void Server::DisconnectFromTcp()
{
    setWindowTitle(QString("TCP并发服务端"));
    QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+"TCP服务停止成功";
    ui->textBrowser_recv->append(msg);
    ui->comboBox_ip->setEnabled(true);
    ui->lineEdit_PORT->setEnabled(true);
    ui->btn_send->setEnabled(false);

    for(int i=0;i<tcpSocket.count();i++){
        qDebug()<<"length:"<<tcpSocket.count();
        tcpSocket.at(i)->abort();
    }
    foreach(QTcpSocket* socket,tcpSocket)
    {
        socket->abort();
    }

    //data clear
    tcpSocket.clear();
    ui->listWidget->clear();
    tcpServer->close();
}



//获取一个当前主机可用IP地址
QHostAddress Server::getLocalHostAddress()
{
    ui->comboBox_ip->clear();
    QHostAddress list;
    list.clear();
    QList<QHostAddress> list_ip = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list_ip)
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
            if(address.toString().contains("192.")) list = address;
            ui->comboBox_ip->addItem(address.toString());
        }
    }
    return list;
}



//服务端总开关
void Server::on_btn_switch_clicked()
{
    static int i=0;
    i++;
    if(i%2!=0)
    {
        tcpListenStatus=true;
        ui->btn_switch->setText(QString("停止服务"));
        ConnectToTcp();
    }
    else
    {
        tcpListenStatus=false;
        ui->btn_switch->setText(QString("启动服务"));
        DisconnectFromTcp();
    }
}



//设置IP地址
void Server::on_comboBox_ip_activated(const QString &arg1)
{
    tcpAddress.setAddress(arg1);
}



//处理连接
void Server::Process_Client_Connect()
{
    //获取新的连接
    QTcpSocket *socket=tcpServer->nextPendingConnection();
    tcpSocket.append(socket);

    //更新listwidget和GroupBox_online
    onlineCount++;
    onlineTitle=QString("在线列表  当前人数:%1").arg(onlineCount);
    ui->groupBox_online->setTitle(onlineTitle);
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/image/images/user.png"),QString("[%1:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort())));

    //更新接受栏
    QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"
            +QString("[%1:%2]连接成功").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    ui->textBrowser_recv->append(msg);

    //发送信息给客户端
    QString clientMsg=QString("server:connect success!");
    socket->write(clientMsg.toUtf8());
    connect(socket,&QTcpSocket::readyRead,this,&Server::Process_Client_Send);
    connect(socket,&QTcpSocket::disconnected,this,&Server::Process_Client_Disconnect);
}



//接受下位机发来的信息
//客户端:需要客户端自己获取系统时间并拼接服务器发送内容
void Server::Process_Client_Send()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"
            +QString("[%1:%2]").arg(socket->peerAddress().toString())
                                 .arg(socket->peerPort());
    QString tcpMsg=socket->readAll();

    //发给wifi界面
    emit sendStm32Data(tcpMsg.toUtf8());
    //发给pwm界面
    if(tcpMsg.at(0)=="p"&&tcpMsg.at(tcpMsg.length()-1)=="m")
    {
        emit sendPWMData(tcpMsg);
    }

    msg+=tcpMsg;
    ui->textBrowser_recv->append(msg.toUtf8());
    MysqlForm::addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),tcpMsg,"wifi");

    //回发  同意将客户端发来的消息回发给客户端，核实数据准确性
    msg=QStringLiteral("server recv:%1").arg(tcpMsg);
    socket->write(msg.toUtf8());
}



//断开连接
void Server::Process_Client_Disconnect()
{
    QTcpSocket* socket=(QTcpSocket*)this->sender();
    QString msg=QString("%1#[%2:%3]断开连接")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))
            .arg(socket->peerAddress().toString())
            .arg(socket->peerPort());

    ui->textBrowser_recv->append(msg);
    for(int i=0;i<ui->listWidget->count();i++)
    {
        QListWidgetItem* item=ui->listWidget->item(i);
        QString info=QString("[%1:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());
        if(item->text()==info){
            ui->listWidget->takeItem(i);
        }
    }
    tcpSocket.removeOne(socket);
    onlineCount--;
    onlineTitle=QString("在线列表  当前人数:%1").arg(onlineCount);
    ui->groupBox_online->setTitle(onlineTitle);
}



//清空接收区
void Server::on_btn_clear_recv_clicked()
{
    ui->textBrowser_recv->clear();
}



//tcp群发
void Server::on_btn_send_clicked()
{
    for(int i=0;i<tcpSocket.count();i++)
    {
        tcpSocket.at(i)->write(ui->textEdit_send->toPlainText().toUtf8());
    }
    QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+QString("[%1:%2:%3]")
                                              .arg(tcpAddress.toString())
                                              .arg(tcpPort)
                                              .arg(QString("群发"))+ui->textEdit_send->toPlainText() ;
    ui->textBrowser_recv->append(msg.toUtf8());
     ui->textEdit_send->clear();
}



//tcp单发
void Server::on_btn_send_one_clicked()
{
    QList<QListWidgetItem*> selectedItems=ui->listWidget->selectedItems();
    foreach(QListWidgetItem* item,selectedItems)
    {
        for(int i=0;i<tcpSocket.count();i++)
        {
            QString info =QString("[%1:%2]").arg(tcpSocket.at(i)->peerAddress().toString()).arg(tcpSocket.at(i)->peerPort());
            if(info==item->text())
            {
                tcpSocket.at(i)->write(ui->textEdit_send->toPlainText().toUtf8());
            }
        }
    }

    QString msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"
            +QString("[%1:%2:%3]").arg(tcpAddress.toString())
                                 .arg(tcpPort).arg(QString("单发"))+ui->textEdit_send->toPlainText();
    MysqlForm::addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),ui->textEdit_send->toPlainText(),"wifi");
    ui->textBrowser_recv->append(msg.toUtf8());
    ui->textEdit_send->clear();
}



//处理WiFi指令
void Server::dealWifiOrder(QString order)
{
    //wifi界面传来的指令发给stm32下位机
    QList<QListWidgetItem*> selectedItems=ui->listWidget->selectedItems();
    foreach(QListWidgetItem* item,selectedItems)
    {
        for(int i=0;i<tcpSocket.count();i++)
        {
            QString info =QString("[%1:%2]").arg(tcpSocket.at(i)->peerAddress().toString()).arg(tcpSocket.at(i)->peerPort());
            if(info==item->text())
            {
                tcpSocket.at(i)->write(order.toUtf8());
            }
        }
    }
    QString msg=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"
            +QString("[%1:%2:%3]").arg(tcpAddress.toString())
                                 .arg(tcpPort).arg(QString("单发"))+order;
    ui->textBrowser_recv->append(msg.toUtf8());
    MysqlForm::addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),order,"wifi");
}
