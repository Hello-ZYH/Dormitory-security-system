#ifndef WIFIFORM_H
#define WIFIFORM_H

#include <QWidget>

namespace Ui
{
class WifiForm;
}

class WifiForm : public QWidget
{
    Q_OBJECT

public:
    explicit WifiForm(QWidget *parent = nullptr);
    ~WifiForm();
public slots:
    void dealStm32Data(QString data);
private slots:
    void on_checkBox_led1_stateChanged(int arg1);

    void on_checkBox_led2_stateChanged(int arg1);

    void on_checkBox_beep_stateChanged(int arg1);

    void on_btn_temp_clicked();

    void on_btn_humi_clicked();

    void on_btn_lock_clicked();

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
private:
    Ui::WifiForm *ui;
    QStringList orderList;
    int type_index;
signals:
    void sendOrder(QString order);
};

#endif // WIFIFORM_H
