#ifndef USARTFORM_H
#define USARTFORM_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QByteArray>
#include <QMessageBox>
#include <QMutex>

namespace Ui
{
class UsartForm;
}

class WifiForm;
class MainWindow;



class UsartForm : public QWidget
{
    Q_OBJECT
    friend class WifiForm;
    friend class MainWindow;

public:
    explicit UsartForm(QWidget *parent = nullptr);
    ~UsartForm();
    void writeOrderToSerialPort(QString order,QString id);

private:
    Ui::UsartForm *ui;
    QSerialPort* serialPort;
    QString MSG_N;
    QString currentWidgetName;
    static QStringList orderList,funcList;
    QMutex m_mutex;

private:
    void initOrderList();
    void ShowDataToWidget(QString data);

public:
    void initSerialPort();

public slots:
    void recvPortName(QString portName);
    void dealCloseSerialPort();

private slots:
    void on_receive_data();

    void on_checkBox_led1_stateChanged(int arg1);

    void on_checkBox_led2_stateChanged(int arg1);

    void on_checkBox_beep_stateChanged(int arg1);

    void on_btn_lock_clicked();

    void on_btn_temp_clicked();

    void on_btn_humi_clicked();

    //void on_btn_light_clicked();

    //void on_btn_batterry_clicked();

    //void on_btn_vr_clicked();

    //void on_btn_pressure_clicked();

    //void on_btn_cpu_clicked();

    //void on_btn_rfid_clicked();

    //void on_btn_up_clicked();

    //void on_btn_down_clicked();

    //void on_btn_left_clicked();

    //void on_btn_right_clicked();

    //void on_btn_esc_clicked();

    //void on_btn_select_clicked();

signals:
    void sendSerialList(QStringList& serialList);
    void SerialPortStatus(bool status,QString error);
    void sendSTM32Data(QString data);
};

#endif // USARTFORM_H
