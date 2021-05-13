#ifndef SERVER_H
#define SERVER_H

#include "mysqlform.h"
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QTime>
#include <QSize>
#include <QListWidgetItem>
#include <QStringLiteral>

#define MAXSIZE 750,800
#define MINSIZE 750,800
#define CURSIZE 750,800
#define TCP_PORT 6666

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Server;
}
QT_END_NAMESPACE



class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();
    bool getTcpListenStatus(){return tcpListenStatus;}
    void RefreshTcpIp();
    void ClearRecv();
    void CloseTcpServer();

private slots:
    void on_btn_switch_clicked();
    void on_comboBox_ip_activated(const QString &arg1);
    void on_btn_clear_recv_clicked();
    void on_btn_send_clicked();
    //myself
    void Process_Client_Connect();
    void Process_Client_Send();
    void Process_Client_Disconnect();
    void on_btn_send_one_clicked();

public slots:
    void dealWifiOrder(QString order);

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> tcpSocket;
    QHostAddress tcpAddress;
    quint16 tcpPort;
    int onlineCount=0;
    bool tcpListenStatus;
    //title
    QString onlineTitle;

private:
    void LoadSetting();
    void ConnectToTcp();
    void DisconnectFromTcp();
    QHostAddress getLocalHostAddress();
signals:
    void sendStm32Data(QString data);
    void sendPWMData(QString data);
};
#endif // SERVER_H
