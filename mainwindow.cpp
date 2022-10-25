#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTime>



bool MainWindow::MainTimerStatus=false;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initQSS();
    ListenAction();
    initHelpContent();
    /*thread*/
    orderThread= new OrderThread(this);
    mytimer = new QTimer(this);

    //每隔10秒挨个发送一次
    mytimer->setInterval(10000);

    //主界面打开串口会触发,发给选中并打开的串口名给usartForm界面，再usartForm界面打开串口QSerialPort
    connect(this,&MainWindow::sendPortName,usartForm,&UsartForm::recvPortName);

    //主界面关闭串口会触发，发给usartForm界面，让其关闭串口QSerialPort
    connect(this,&MainWindow::closeSerialPort,usartForm,&UsartForm::dealCloseSerialPort);

    //主线程的interval信号槽
    connect(mytimer,&QTimer::timeout,this,&MainWindow::dealMainThread);

    //子进程的任务执行信号
    connect(orderThread,&OrderThread::sendUartOrder,this,&MainWindow::dealThreadOrder);
}



MainWindow::~MainWindow()
{
    //退出前关闭所有线程
    mytimer->stop();
    orderThread->stop();
    orderThread->requestInterruption();
    orderThread->quit();
    orderThread->wait();

    //关闭TCP服务
    tcpServerForm->CloseTcpServer();
    tcpServerForm->close();

    delete mytimer;
    delete tcpServerForm;
    delete usartForm;
    delete pwmForm;
    delete mysqlForm;
    delete wifiForm;
    //delete cameraForm;
    delete ui;
}



void MainWindow::initQSS()
{
    wifiForm = new WifiForm(this);
    usartForm = new UsartForm(this);
    mysqlForm = new MysqlForm(this);
    pwmForm = new PWMForm(this,3.3,0);                            //给定最高电压3.3V,当前接受数据总数0条
    tcpServerForm = new Server(this);
    //cameraForm = new CameraForm(this);
    //pictureForm = new PictureForm(this);

    ui->tabWidget->addTab(usartForm,QString("串口控制界面"));
    ui->tabWidget->addTab(wifiForm,QString("wifi控制界面"));
    ui->tabWidget->addTab(mysqlForm,QString("数据库界面"));
    ui->tabWidget->addTab(pwmForm,QString("PWM调节界面"));
    ui->tabWidget->addTab(tcpServerForm,QString("TCP服务器"));
    //ui->tabWidget->addTab(cameraForm,QString("报警图片"));
    //ui->tabWidget->addTab(pictureForm,QString("图片"));
    //ui->tabWidget->setCurrentWidget(usartForm);

    //action
    //ui->action_vr->setChecked(false);
    //ui->action_CPU->setChecked(false);
    //ui->action_RFID->setChecked(false);
    //ui->action_line->setChecked(false);
    ui->action_temp->setChecked(false);
    ui->action_thread->setChecked(false);
    //ui->action_batterry->setChecked(false);
    //ui->action_pressure->setChecked(false);

    //btn
    ui->btn_save_library->setProperty("level","normal");
    ui->btn_clear_recv->setProperty("level","normal");;
    ui->btn_open_usart->setProperty("level","normal");
    ui->btn_refresh_com->setProperty("level","normal");

    //qss
    QFile file(":/qss/QSS/mainwindow.qss");
    file.open(QIODevice::ReadOnly);
    QString qss = file.readAll();
    setStyleSheet(qss);
    file.close();
    setWindowTitle(QString("宿舍智能物联网系统"));
}



//帮助
void MainWindow::initHelpContent()
{
    orderHelpContent.clear();
    orderHelpContent="<b>";
    comHelpContent=QString("<b>1.将程序下载到开发板。<br>2.打开串口。<br>3.开启子线程。<br>4.开启总线程。</b>");
    wifiHelpContent=QString("<b>1.将程序下载到开发板。<br>2.打开TCP服务。<br>3.按下板子OK键。<br>4.选中要发送的客户端。<br>5.切换到wifi控制界面。<br>6.点击想要获取相关数据的刷新按钮。</b>");

    for(int i=0;i<UsartForm::orderList.count();i++)
    {
        orderHelpContent+=QString("%1.%2--------<a style='color:red'>%3</a><hr>").arg(i+1).arg(UsartForm::orderList.at(i)).arg(UsartForm::funcList.at(i));
    }
    orderHelpContent+="</b>";
}



//监听数据
void MainWindow::ListenAction()
{
    /****************************usartFrom signals*********************************************/
    //监听usartForm发来的采集到的可用端口号序列，更新Mainwindow的端口QCombox
    connect(usartForm,SIGNAL(sendSerialList(QStringList&)),this,SLOT(recvSerialList(QStringList&)));

    //监听usartform发来的串口open函数的信息
    connect(usartForm,&UsartForm::SerialPortStatus,this,&MainWindow::TipsSerialPortStatus);

    //usart发来的数据同步到数据库和textBrowser上
    connect(usartForm,&UsartForm::sendSTM32Data,this,&MainWindow::dealSTM32Data);


    /****************************wifiForm signals*********************************************/
    //wifi界面发来的指令转发给tcp界面
    connect(wifiForm,&WifiForm::sendOrder,tcpServerForm,&Server::dealWifiOrder);


    /****************************tcpServer signals*********************************************/
    //tcp发来的32的数据发送给wifi界面
    connect(tcpServerForm,&Server::sendStm32Data,wifiForm,&WifiForm::dealStm32Data);

    //tcp发来的32的pwm数据发送给pwm界面
    //connect(tcpServerForm,&Server::sendPWMData,pwmForm,&PWMForm::dealPWMData);

    //tcp发来的32的数据发送给camera界面
    //connect(tcpServerForm,&Server::sendCameraData,cameraForm,&CameraForm::dealStm32Data);
    //connect(tcpServerForm,&Server::sendPictureData,pictureForm,&PictureForm::);
    //connect(tcpServerForm, &Server::sendgetImageData,pictureForm, &PictureForm::getImage);


    /****************************MainWindow signals*********************************************/
    //菜单栏
    connect(ui->action_refresh_com,&QAction::triggered,[=](bool){
        on_btn_refresh_com_clicked();
    });
    connect(ui->action_clear_library,&QAction::triggered,[=](){
        on_btn_clear_recv_clicked();
    });
    connect(ui->action_IP,&QAction::triggered,[=](){
        tcpServerForm->RefreshTcpIp();
    });
    connect(ui->action_clear_recv,&QAction::triggered,[=](){
       tcpServerForm->ClearRecv();
    });
    connect(ui->action_PWM,&QAction::triggered,[=](){
       pwmForm->refreshChart();
    });
    connect(ui->action_PWM,&QAction::triggered,[=](){
       pwmForm->refreshChart();
    });
    connect(ui->action_mysql,&QAction::triggered,[=](){
       initMysql();
    });
    connect(ui->action_order_use,&QAction::triggered,[=](){
        CreateHelpWidget(orderHelpContent);
    });
    connect(ui->action_com_help,&QAction::triggered,[=](){
        CreateHelpWidget(comHelpContent);
    });
    connect(ui->action_wifi_help,&QAction::triggered,[=](){
        CreateHelpWidget(wifiHelpContent);
    });

    //总线程开关
    connect(ui->action_thread,&QAction::triggered,[=](bool status){
        if(status)
        {
            MainTimerStatus=true;
            ui->textBrowser_recv->append(QTime::currentTime().toString()+"#"+"主线程已启动!");
            ui->action_thread->setText("关闭总线程");
            mytimer->start();
            orderThread->start();
        }
        else
        {
            MainTimerStatus=false;
            ui->textBrowser_recv->append(QTime::currentTime().toString()+"#"+"主线程已关闭!");
            ui->action_thread->setText("开启总线程");
            mytimer->stop();
            orderThread->stop();
            //取消子线程的选中状态
            //ui->action_vr->setChecked(false);
            // ui->action_CPU->setChecked(false);
            // ui->action_RFID->setChecked(false);
            //ui->action_line->setChecked(false);
            ui->action_temp->setChecked(false);
            ui->action_thread->setChecked(false);
            //ui->action_batterry->setChecked(false);
            //ui->action_pressure->setChecked(false);
        }
    });
    usartForm->initSerialPort();
}



 //连接数据库
void MainWindow::initMysql()
{
    stm32Mysql::initMysql();
    mysqlForm->initMysqlData();

    if(!stm32Mysql::getConnectStatus())
    {
       QMessageBox::warning(this,"数据库提示",stm32Mysql::getOpenMysqlError());
       return;
    }
    else
    {
        ui->textBrowser_recv->append(QTime::currentTime().toString()+"#"+"数据库连接成功!日志将自动同步到数据库中");
        mysqlForm->addDataToLibrary(QTime::currentTime().toString(),"数据库连接成功!日志将自动同步到数据库中","串口");
    }
}



//帮助窗口
void MainWindow::CreateHelpWidget(QString helpContent)
{
    QWidget* helpWdiget = new QWidget();
    helpWdiget->setWindowTitle("帮助");
    helpWdiget->setMinimumSize(300,300);
    helpWdiget->setMaximumSize(300,300);
    helpWdiget->setGeometry(this->x()+300,this->y()+300,0,0);
    QGridLayout* gridLayout=new QGridLayout;
    QTextBrowser* browser = new QTextBrowser(helpWdiget);
    browser->append(helpContent);
    gridLayout->addWidget(browser);
    helpWdiget->setLayout(gridLayout);
    helpWdiget->show();
}



//打开串口按钮
void MainWindow::on_btn_open_usart_clicked()
{
    static int i=0;
    i++;
    if(i%2!=0)
    {
        emit sendPortName(ui->comboBox_com->currentText());
    }
    else
    {
        emit closeSerialPort();
        ui->btn_open_usart->setText("打开串口");
        ui->textBrowser_recv->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+"关闭串口成功!");
    }
}



//刷新串口按钮
void MainWindow::recvSerialList(QStringList& serialList)
{
    ui->comboBox_com->clear();
    ui->textBrowser_recv->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+"刷新串口成功!");
    ui->comboBox_com->addItems(serialList);
}



//串口状态
void MainWindow::TipsSerialPortStatus(bool status,QString error)
{
    if(status)
    {
        QMessageBox::warning(this,"tips","打开串口成功!");
        ui->btn_open_usart->setText("关闭串口");
        ui->textBrowser_recv->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+"打开串口成功!");
        mysqlForm->addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),"打开串口成功!","串口");
        return ;
    }
    else
    {
        QMessageBox::warning(this,"tips","打开串口失败:"+error);
        ui->textBrowser_recv->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+"打开串口失败!"+error);
        mysqlForm->addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),"打开串口失败!"+error,"串口");
        return ;
    }
}



void MainWindow::on_btn_refresh_com_clicked()
{
    if(usartForm!=NULL)
        usartForm->initSerialPort();
}



//清空日志
void MainWindow::on_btn_clear_recv_clicked()
{
    ui->textBrowser_recv->clear();
}



//判断处理类型
void MainWindow::dealSTM32Data(QString data)
{
    ui->textBrowser_recv->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+"#"+data);

    if(tcpServerForm->getTcpListenStatus())
        mysqlForm->addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),data,"wifi");
    else
        mysqlForm->addDataToLibrary(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"),data,"串口");
}



//线程
void MainWindow::dealMainThread()
{
    /*
    orderList<<"a1b"<<"a2b"<<"a3b"
             <<"a4b"<<"a5b"<<"a6b"
             <<"a7b"<<"a8b"<<"a9b"
             <<"aAb"<<"aBb"<<"aCb"
             <<"aDb";
    */

    //总线程:负责采集子线程开关状态并加入线程的任务Task堆栈中
    if(!orderThread->isListEmpty()) return ;
//    if(ui->action_batterry->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(6),"6"));
//    }
    if(ui->action_temp->isChecked())
    {
        orderThread->addTask(new Task(UsartForm::orderList.at(6),"6"));
    }
//    if(ui->action_temp->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(7),"7"));
//    }
//    if(ui->action_line->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(8),"8"));
//    }
//    if(ui->action_pressure->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(9),"9"));
//    }
//    if(ui->action_vr->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(10),"10"));
//    }
//    if(ui->action_CPU->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(11),"11"));
//    }
//    if(ui->action_RFID->isChecked())
//    {
//        orderThread->addTask(new Task(UsartForm::orderList.at(12),"12"));
//    }
}



//处理线程采集的信号
void MainWindow::dealThreadOrder(QString order,QString id)
{
    //采集子线程的执行任务信号，将任务的指令和id号发给串口界面执行
    usartForm->writeOrderToSerialPort(order,id);
//    wifiForm->writeOrder(order,id);
}

