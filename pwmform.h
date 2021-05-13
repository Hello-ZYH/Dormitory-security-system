#ifndef PWMFORM_H
#define PWMFORM_H

#include <QWidget>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE


namespace Ui
{
class PWMForm;
}

class PWMForm : public QWidget
{
    Q_OBJECT

public:
    explicit PWMForm(QWidget *parent = nullptr,qreal maxsize=3.3,qreal ctime=0);
    ~PWMForm();
    void refreshChart();
public slots:
    void  dealPWMData(QString data);
private:
    void initCharts();
private:
    Ui::PWMForm *ui;
    QChart* m_Chart;
    QTimer* m_Timer;
    QValueAxis* m_axisX;//x坐标
    QValueAxis* m_axisY;//y坐标
    QLineSeries* m_lineSeries;
    qreal c_MaxSize,c_time;
    int x_Max;
    QVector<QPointF> VoltageData;
};

#endif // PWMFORM_H
