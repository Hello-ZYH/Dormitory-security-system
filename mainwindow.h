#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QWidget>
#include <QGridLayout>
#include <QTextBrowser>
#include "usartform.h"
#include "wifiform.h"
#include "stm32mysql.h"
#include "mysqlform.h"
#include "pwmform.h"
#include "server.h"
#include "orderthread.h"


QT_BEGIN_NAMESPACE
enum ThreadType{Thread_TempHumi,Thread_Battery,Thread_LightLine,\
               Thread_Vr,Thread_Cpu,Thread_Pressure,Thread_RFID};
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static bool MainTimerStatus;//存储总窗体的总线程状态,供其他widget获取
private slots:
    void on_btn_open_usart_clicked();
    void recvSerialList(QStringList& serialList);
    void TipsSerialPortStatus(bool status,QString error);
    void on_btn_refresh_com_clicked();

    void on_btn_clear_recv_clicked();
    void dealSTM32Data(QString data);
    void dealMainThread();
    void dealThreadOrder(QString order,QString id);
private:
    Ui::MainWindow *ui;
    UsartForm* usartForm;
    WifiForm* wifiForm;
    MysqlForm* mysqlForm;
    PWMForm* pwmForm;
    Server* tcpServerForm;
    OrderThread* orderThread;
    QTimer* mytimer;
    QString comHelpContent,wifiHelpContent,orderHelpContent;
private:
    void initQSS();
    void initUsart();
    void initHelpContent();
    void ListenAction();
    void initMysql();
    void CreateHelpWidget(QString helpContent);
signals:
    void sendPortName(QString portName);
    void closeSerialPort();
    void refreshTcpIp();
};
#endif // MAINWINDOW_H
