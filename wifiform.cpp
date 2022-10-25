#include "wifiform.h"
#include "ui_wifiform.h"
#include "usartform.h"
#include <QDebug>
#include <QMetaType>



WifiForm::WifiForm(QWidget *parent) : QWidget(parent), ui(new Ui::WifiForm)
{
    ui->setupUi(this);
    type_index=0;
}



WifiForm::~WifiForm()
{
    delete ui;
}



//处理stm32数据
void WifiForm::dealStm32Data(QString stm32Data)
{
//    qDebug()<<stm32Data<<type_index;
//    switch(type_index)

    qDebug()<<stm32Data.at(1).toLatin1();
    switch (stm32Data.at(1).toLatin1())
    {
    case 'B':
    {
        //cB24.5Cd   cC45%d
        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，1个包尾
        ui->lcdNumber_temp->display(data);
    }break;

    case 'C':
    {
        //cB24.5Cd   cC45%d
        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，1个包尾
        ui->lcdNumber_humi->display(data);
    }break;

    default:break;

//    case 6:
//    {
//        //cA4.04vd
//        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
//        ui->lcdNumber_battery->display(data);
//    }break;

//    case 7:
//    {
//        //cB 24.5C d         cC 45.1% d
//        static int count=0;
//        count++;
//        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾

//        if(count%2!=0)
//            ui->lcdNumber_temp->display(data);
//        else
//            ui->lcdNumber_humi->display(data);
//    }break;

//    case 8:
//    {
//        //cDl3456luxd
//        QString data = stm32Data.mid(2,stm32Data.length()-6);//2个包头，4个包尾
//        ui->lcdNumber_line->display(data);
//    }break;
//    case 9:
//    {
//        //cE3456KPad
//        QString data = stm32Data.mid(2,stm32Data.length()-6);//2个包头，4个包尾
//        ui->lcdNumber_pressure->display(data);
//    }break;
//    case 10:
//    {
//        //cF3.1vd
//        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，2个包尾
//        ui->lcdNumber_vr->display(data);
//    }break;
//    case 11:
//    {
//        //cG34.5Cd
//        QString data = stm32Data.mid(2,stm32Data.length()-4);//2个包头，2个包尾
//        ui->lcdNumber_cpu->display(data);
//    }break;
//    case 12:
//    {
//        //
//        QString data = stm32Data.mid(2,stm32Data.length()-3);//2个包头，1个包尾
//        ui->label_rfid->setText(data);
//    } break;
    }
}



//void WifiForm::writeOrder(QString order,QString id)
//{
////        currentWidgetName=orderList.at(id.toInt()).toUtf8();
////        serialPort->write(order.toUtf8());
//      emit sendOrder(orderList.at(id.toInt()).toUtf8());
////    emit sendOrder(QString(UsartForm::orderList.at(6)));
////    emit sendOrder(QString(UsartForm::orderList.at(7)));
//}



//LED1
void WifiForm::on_checkBox_led1_stateChanged(int arg1)
{
    if(arg1>0)
    {
        type_index=0;
        emit sendOrder(QString(UsartForm::orderList.at(0)));
        ui->checkBox_led1->setIcon(QIcon(":/image/images/led1.png"));
    }
    else
    {
        type_index=1;
        emit sendOrder(QString(UsartForm::orderList.at(1)));
        ui->checkBox_led1->setIcon(QIcon(":/image/images/led2.png"));
    }
}



//LED2
void WifiForm::on_checkBox_led2_stateChanged(int arg1)
{
    if(arg1>0)
    {
        //led2 on
        type_index=2;
        emit sendOrder(QString(UsartForm::orderList.at(2)));
        ui->checkBox_led2->setIcon(QIcon(":/image/images/led1.png"));
    }
    else
    {
        //led2 off
        type_index=3;
        emit sendOrder(QString(UsartForm::orderList.at(3)));
        ui->checkBox_led2->setIcon(QIcon(":/image/images/led2.png"));
    }
}



//蜂鸣器
void WifiForm::on_checkBox_beep_stateChanged(int arg1)
{
    if(arg1>0)
    {
        //beep on
        type_index=4;
        emit sendOrder(QString(UsartForm::orderList.at(4)));
        ui->checkBox_beep->setIcon(QIcon(":/image/images/nbeep1.png"));
    }
    else
    {
        //beep off
        type_index=5;
        emit sendOrder(QString(UsartForm::orderList.at(5)));
        ui->checkBox_beep->setIcon(QIcon(":/image/images/nbeep2.png"));
    }
}



//锂电池
//void WifiForm::on_btn_batterry_clicked()
//{
//    type_index=6;
//    emit sendOrder(QString(UsartForm::orderList.at(6)));
//}



//温度传感器
void WifiForm::on_btn_temp_clicked()
{
    type_index=6;
    emit sendOrder(QString(UsartForm::orderList.at(6)));
}



//湿度传感器
void WifiForm::on_btn_humi_clicked()
{
    type_index=7;
    emit sendOrder(QString(UsartForm::orderList.at(7)));
}



//远程开锁
void WifiForm::on_btn_lock_clicked()
{
    type_index=8;
    emit sendOrder(QString(UsartForm::orderList.at(8)));
}



//远程开窗
void WifiForm::on_btn_window_clicked()
{
    type_index=9;
    emit sendOrder(QString(UsartForm::orderList.at(9)));
}



//光线传感器
//void WifiForm::on_btn_light_clicked()
//{
//    type_index=8;
//    emit sendOrder(QString(UsartForm::orderList.at(8)));
//}



//大气压传感器
//void WifiForm::on_btn_pressure_clicked()
//{
//    type_index=9;
//    emit sendOrder(QString(UsartForm::orderList.at(9)));
//}



//电位器
//void WifiForm::on_btn_vr_clicked()
//{
//    type_index=10;
//    emit sendOrder(QString(UsartForm::orderList.at(10)));
//}



//CPU温度
//void WifiForm::on_btn_cpu_clicked()
//{
//    type_index=11;
//    emit sendOrder(QString(UsartForm::orderList.at(11)));
//}



//RFID
//void WifiForm::on_btn_rfid_clicked()
//{
//    type_index=12;
//    emit sendOrder(QString(UsartForm::orderList.at(12)));
//}



//上键
//void WifiForm::on_btn_up_clicked()
//{
//    currentWidgetName=orderList.at(13).toUtf8();
//    serialPort->write(orderList.at(13).toUtf8());
//    emit sendSTM32Data(orderList.at(13).toUtf8());
//}



//下键
//void WifiForm::on_btn_down_clicked()
//{
//    currentWidgetName=orderList.at(14).toUtf8();
//    serialPort->write(orderList.at(14).toUtf8());
//    emit sendSTM32Data(orderList.at(14).toUtf8());
//}



//左键
//void WifiForm::on_btn_left_clicked()
//{
//    currentWidgetName=orderList.at(15).toUtf8();
//    serialPort->write(orderList.at(15).toUtf8());
//    emit sendSTM32Data(orderList.at(15).toUtf8());
//}



//右键
//void WifiForm::on_btn_right_clicked()
//{
//    currentWidgetName=orderList.at(16).toUtf8();
//    serialPort->write(orderList.at(16).toUtf8());
//    emit sendSTM32Data(orderList.at(16).toUtf8());
//}



//退出键
//void WifiForm::on_btn_esc_clicked()
//{
//    currentWidgetName=orderList.at(17).toUtf8();
//    serialPort->write(orderList.at(17).toUtf8());
//    emit sendSTM32Data(orderList.at(17).toUtf8());
//}



//选择键
//void WifiForm::on_btn_select_clicked()
//{

//}
