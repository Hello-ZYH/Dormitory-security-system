/*
包头： 命令的头 ： 'a'
包尾： 命令结束 ： 'b'

led1亮 ：    "a1b"         成功："c1sd"                               失败："c1fd"
led1灭 ：    "a2b"         成功："c2sd"                               失败："c2fd"
led2亮 ：    "a3b"         成功："c3sd"                               失败："c3fd"
led2灭 ：    "a4b"         成功："c4sd"                               失败："c4fd"
beep响 ：    "a5b"         成功："c5sd"                               失败："c5fd"
beep灭 ：    "a6b"         成功："c6sd"                               失败："c6fd"
锂电池电压：  "a7b"         成功："cA4.04vd"                           失败："cAfd"
温湿度传感器： "a8b"         成功：温度"cB24.5Cd"       湿度"cC45%d"     失败：温度"cBfd"   湿度"cCfd"
光线传感器：  "a9b"         成功："cDl3456luxd"                        失败："cDfd"
大气压传感器： "aAb"         成功："cE3456KPad"                         失败："cEfd"
电位器：      "aBb"         成功："cF3.1vd"                            失败："cFfd"
cpu温度：     "aCb"         成功："cG34.5Cd"                           失败："cGfd"
RFID卡：     "aDb"         成功："cH34.5Cd"                           失败："cHfd"
查询按键："aEb"
成功："cIupd"
"cIdownd"
"cIleftd"
"cIrightd"
"cIokd"
"cIescd"
失败："cIfd"
*/

#include "usartform.h"
#include "ui_usartform.h"
#include <QDebug>
#include <QMetaType>


QStringList UsartForm::orderList;
QStringList UsartForm::funcList;

UsartForm::UsartForm(QWidget *parent) : QWidget(parent), ui(new Ui::UsartForm)
{
    ui->setupUi(this);
    initOrderList();
}



UsartForm::~UsartForm()
{
    delete ui;
}



//数据列表
void UsartForm::initOrderList()
{
    orderList   <<"a1b"
                <<"a2b"
                <<"a3b"
                <<"a4b"
                <<"a5b"
                <<"a6b"
                <<"a7b"
                <<"a8b"
                <<"a9b";
                //<<"aAb"
                //<<"aBb"
                //<<"aCb"
                //<<"aDb";

//    orderList   <<"aEb";//ok

    funcList    <<"打开LED1"
                <<"关闭LED1"
                <<"打开LED2"
                <<"关闭LED2"
                <<"打开蜂鸣器"
                <<"关闭蜂鸣器"  
                <<"获取温度"
                <<"获取湿度"
                <<"远程开锁";
                //<<"获取锂电池电压"
                //<<"获取光线亮度"
                //<<"获取大气压压强"
                //<<"获取电位器电压"
                //<<"获取CPU温度"
                //<<"获取RFID卡ID"
                //<<"发送OK按键";
MSG_N.clear();
}



//显示数据
void UsartForm::ShowDataToWidget(QString stm32Data)
{
    //不符合协议数据
    if(stm32Data.length()<4) return;

    m_mutex.lock();
/*方案1有bug*/
#if 0

    switch (orderList.lastIndexOf(currentWidgetName))
    {
    case 6:
    {
        //cA4.04vd
        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
        ui->lcdNumber_battery->display(data);
    }break;
    case 7:
    {
        //cB24.5Cd   cC45%d
        static int count=0;
        count++;
        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
        if(count%2!=0)
            ui->lcdNumber_temp->display(data);
        else
            ui->lcdNumber_humi->display(data);
    }break;
    case 8:
    {
        //cDl3456luxd
        QString data = stm32Data.mid(2,stm32Data.length()-6);//2个包头，4个包尾
        ui->lcdNumber_line->display(data);
    }break;
    case 9:
    {
        //cE3456KPad
        QString data = stm32Data.mid(2,stm32Data.length()-6);//2个包头，4个包尾
        ui->lcdNumber_pressure->display(data);
    }break;
    case 10:
    {
        //cF3.1vd
        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，2个包尾
        ui->lcdNumber_vr->display(data);
    }break;
    case 11:
    {
        //cG34.5Cd
        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，2个包尾
        ui->lcdNumber_cpu->display(data);
    }break;
    case 12:
    {
        //
        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
        ui->label_rfid->setText(data);
    } break;
    default:break;
    }

#endif
    /*方案2*/
#if 1
    qDebug()<<stm32Data.at(1).toLatin1();
    switch (stm32Data.at(1).toLatin1())
    {
//    case 'A':
//    {
//        //cA4.04vd
//        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
//        ui->lcdNumber_battery->display(data);
//    }break;
    case 'B':
    {
        //cB24.5Cd   cC45%d
        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
        ui->lcdNumber_temp->display(data);
    }break;
    case 'C':
    {
        //cB24.5Cd   cC45%d
        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
        ui->lcdNumber_humi->display(data);
    }break;
//    case 'D':
//    {
//        //cDl3456luxd
//        QString data = stm32Data.mid(2,stm32Data.length()-6);//2个包头，4个包尾
//        ui->lcdNumber_line->display(data);
//    }break;
//    case 'E':
//    {
//        //cE3456KPad
//        QString data = stm32Data.mid(2,stm32Data.length()-6);//2个包头，4个包尾
//        ui->lcdNumber_pressure->display(data);
//    }break;
//    case 'F':
//    {
//        //cF3.1vd
//        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，2个包尾
//        ui->lcdNumber_vr->display(data);
//    }break;
//    case 'G':
//    {
//        //cG34.5Cd
//        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，2个包尾
//        ui->lcdNumber_cpu->display(data);
//    }break;
//    case 'H':
//    {
//        //
//        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
//        ui->label_rfid->setText(data);
//    } break;
    default:break;
    }
#endif
    m_mutex.unlock();
}



//初始化串口
void UsartForm::initSerialPort()
{
    //获取所有com口
    QStringList serialList;
    serialPort = new QSerialPort(this);
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            serialList.append(serial.portName());
            serial.close();
        }
    }

    //发送信号给主窗口所有的com口
    emit sendSerialList(serialList);
    //设置串口
    serialPort->setBaudRate(QSerialPort::Baud115200);      //设置波特率为115200
    serialPort->setDataBits(QSerialPort::Data8);           //设置数据位8
    serialPort->setParity(QSerialPort::NoParity);          //校验位设置为0
    serialPort->setStopBits(QSerialPort::OneStop);         //停止位设置为1
    serialPort->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

    connect(serialPort,&QSerialPort::readyRead,this,&UsartForm::on_receive_data);
}



//接收串口名
void UsartForm::recvPortName(QString portName)
{
    serialPort->setPortName(portName);
    if(serialPort->open(QIODevice::ReadWrite))
    {
        emit SerialPortStatus(true,"");
        return ;
    }
    else
    {
        emit SerialPortStatus(false,serialPort->errorString());
        return ;
    }
}



//接收数据
void UsartForm::on_receive_data()
{
    QByteArray msg = serialPort->readAll();
    MSG_N+=msg;
    if(MSG_N.lastIndexOf("d")==MSG_N.length()-1)
    {
        emit sendSTM32Data(MSG_N);
        ShowDataToWidget(MSG_N);
        if(!MSG_N.contains("H"))
            MSG_N.clear();
    }
    else if(MSG_N.lastIndexOf("D")==MSG_N.length()-1)
    {
        emit sendSTM32Data(MSG_N);
        ShowDataToWidget(MSG_N);
            MSG_N.clear();
    }
}



//关闭串口
void UsartForm::dealCloseSerialPort()
{
    serialPort->close();
}



void UsartForm::writeOrderToSerialPort(QString order,QString id)
{
    if(serialPort->isOpen())
    {
        currentWidgetName=orderList.at(id.toInt()).toUtf8();
        serialPort->write(order.toUtf8());
        emit sendSTM32Data(orderList.at(id.toInt()).toUtf8());
    }
}



//LED1
void UsartForm::on_checkBox_led1_stateChanged(int arg1)
{
    if(arg1>0)
    {
        //led1 on
        currentWidgetName=orderList.at(0).toUtf8();
        serialPort->write(orderList.at(0).toUtf8());
        emit sendSTM32Data(orderList.at(0).toUtf8());
        ui->checkBox_led1->setIcon(QIcon(":/image/images/led1.png"));
    }
    else
    {
        //led1 off
        currentWidgetName=orderList.at(1).toUtf8();
        serialPort->write(orderList.at(1).toUtf8());
        emit sendSTM32Data(orderList.at(1).toUtf8());
        ui->checkBox_led1->setIcon(QIcon(":/image/images/led2.png"));
    }
}



//LED2
void UsartForm::on_checkBox_led2_stateChanged(int arg1)
{
    if(arg1>0)
    {
        //led2 on
        currentWidgetName=orderList.at(2).toUtf8();
        serialPort->write(orderList.at(2).toUtf8());
        emit sendSTM32Data(orderList.at(2).toUtf8());
        ui->checkBox_led2->setIcon(QIcon(":/image/images/led1.png"));
    }
    else
    {
        //led2 off
        currentWidgetName=orderList.at(3).toUtf8();
        serialPort->write(orderList.at(3).toUtf8());
        emit sendSTM32Data(orderList.at(3).toUtf8());
        ui->checkBox_led2->setIcon(QIcon(":/image/images/led2.png"));
    }
}



//蜂鸣器
void UsartForm::on_checkBox_beep_stateChanged(int arg1)
{
    if(arg1>0)
    {
        //beep on
        currentWidgetName=orderList.at(4).toUtf8();
        serialPort->write(orderList.at(4).toUtf8());
        emit sendSTM32Data(orderList.at(4).toUtf8());
        ui->checkBox_beep->setIcon(QIcon(":/image/images/nbeep1.png"));
    }
    else
    {
        //beep off
        currentWidgetName=orderList.at(5).toUtf8();
        serialPort->write(orderList.at(5).toUtf8());
        emit sendSTM32Data(orderList.at(5).toUtf8());
        ui->checkBox_beep->setIcon(QIcon(":/image/images/nbeep2.png"));
    }
}



//锂电池
//void UsartForm::on_btn_batterry_clicked()
//{
//    currentWidgetName=orderList.at(6).toUtf8();
//    serialPort->write(orderList.at(6).toUtf8());
//    emit sendSTM32Data(orderList.at(6).toUtf8());
//}



//温度
void UsartForm::on_btn_temp_clicked()
{
    currentWidgetName=orderList.at(6).toUtf8();
    serialPort->write(orderList.at(6).toUtf8());
    emit sendSTM32Data(orderList.at(6).toUtf8());
}



//湿度
void UsartForm::on_btn_humi_clicked()
{
    currentWidgetName=orderList.at(7).toUtf8();
    serialPort->write(orderList.at(7).toUtf8());
    emit sendSTM32Data(orderList.at(7).toUtf8());
}



//远程开锁
void UsartForm::on_btn_lock_clicked()
{
    currentWidgetName=orderList.at(8).toUtf8();
    serialPort->write(orderList.at(8).toUtf8());
    emit sendSTM32Data(orderList.at(8).toUtf8());
    ui->btn_lock->setIcon(QIcon(":/image/images/lock1.png"));
}



//光线传感器
//void UsartForm::on_btn_light_clicked()
//{
//    currentWidgetName=orderList.at(9).toUtf8();
//    serialPort->write(orderList.at(9).toUtf8());
//    emit sendSTM32Data(orderList.at(9).toUtf8());
//}



//大气压传感器
//void UsartForm::on_btn_pressure_clicked()
//{
//    currentWidgetName=orderList.at(10).toUtf8();
//    serialPort->write(orderList.at(10).toUtf8());
//    emit sendSTM32Data(orderList.at(10).toUtf8());
//}



//电位器
//void UsartForm::on_btn_vr_clicked()
//{
//    currentWidgetName=orderList.at(11).toUtf8();
//    serialPort->write(orderList.at(11).toUtf8());
//    emit sendSTM32Data(orderList.at(11).toUtf8());
//}



//CPU温度
//void UsartForm::on_btn_cpu_clicked()
//{
//    currentWidgetName=orderList.at(12).toUtf8();
//    serialPort->write(orderList.at(12).toUtf8());
//    emit sendSTM32Data(orderList.at(12).toUtf8());
//}



//RDid
//void UsartForm::on_btn_rfid_clicked()
//{
//    currentWidgetName=orderList.at(13).toUtf8();
//    serialPort->write(orderList.at(13).toUtf8());
//    emit sendSTM32Data(orderList.at(13).toUtf8());
//}



//上键
//void UsartForm::on_btn_up_clicked()
//{
//    currentWidgetName=orderList.at(13).toUtf8();
//    serialPort->write(orderList.at(13).toUtf8());
//    emit sendSTM32Data(orderList.at(13).toUtf8());
//}



//下键
//void UsartForm::on_btn_down_clicked()
//{
//    currentWidgetName=orderList.at(14).toUtf8();
//    serialPort->write(orderList.at(14).toUtf8());
//    emit sendSTM32Data(orderList.at(14).toUtf8());
//}



//左键
//void UsartForm::on_btn_left_clicked()
//{
//    currentWidgetName=orderList.at(15).toUtf8();
//    serialPort->write(orderList.at(15).toUtf8());
//    emit sendSTM32Data(orderList.at(15).toUtf8());
//}



//右键
//void UsartForm::on_btn_right_clicked()
//{
//    currentWidgetName=orderList.at(16).toUtf8();
//    serialPort->write(orderList.at(16).toUtf8());
//    emit sendSTM32Data(orderList.at(16).toUtf8());
//}



//退出键
//void UsartForm::on_btn_esc_clicked()
//{
//    currentWidgetName=orderList.at(17).toUtf8();
//    serialPort->write(orderList.at(17).toUtf8());
//    emit sendSTM32Data(orderList.at(17).toUtf8());
//}



//确认键
//void UsartForm::on_btn_select_clicked()
//{
//    currentWidgetName=orderList.at(14).toUtf8();
//    serialPort->write(orderList.at(14).toUtf8());
//    emit sendSTM32Data(orderList.at(14).toUtf8());
//}
