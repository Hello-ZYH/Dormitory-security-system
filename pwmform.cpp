#include "pwmform.h"
#include "ui_pwmform.h"



PWMForm::PWMForm(QWidget *parent,qreal maxsize,qreal ctime) : QWidget(parent), ui(new Ui::PWMForm), c_MaxSize(maxsize), c_time(ctime)
{
    ui->setupUi(this);
    x_Max=10;
    initCharts();
}



PWMForm::~PWMForm()
{
    delete ui;
}



//更新Charts
void PWMForm::refreshChart()
{
    m_lineSeries->clear();
    c_time=0;
    x_Max=10;
    m_axisX->setMax(x_Max);
    m_axisX->setMin(0);//更新x轴的最大最小值
}



//处理PWM数据
void PWMForm::dealPWMData(QString data)
{
    data = data.mid(1,data.length()-2);//2个包头，2个包尾
    c_time++;
    if(c_time>x_Max)
    {
        x_Max++;
        m_axisX->setMax(x_Max);
        m_axisX->setMin(m_axisX->min()+1);//更新x轴的最大最小值
    }
    m_lineSeries->append(QPointF(c_time,data.toFloat()));
}



//初始化Charts
void PWMForm::initCharts()
{
    //构建折线系列
    m_lineSeries=new QLineSeries(this);
    //构建图表对象
    m_Chart=new QChart();
    //将折线系列添加到图表
    m_Chart->addSeries(m_lineSeries);

    //设计自己的x,y坐标
    m_axisX=new QValueAxis();
    m_axisX->setRange(0,x_Max);
    m_axisX->setTitleText("秒(S)");
    m_axisX->setLabelFormat("%g");
    m_axisX->setTickCount(x_Max+1);//设置刻度数 10piece
    m_axisY=new QValueAxis();
    m_axisY->setRange(0,c_MaxSize);
    m_axisY->setTickCount(11);//设置刻度数 10piece
    m_axisY->setTitleText("电压(V)");

    //将坐标轴与折线系列绑定
    m_Chart->setAxisX(m_axisX,m_lineSeries);
    m_Chart->setAxisY(m_axisY,m_lineSeries);
    //设置图表主题
    m_Chart->setTheme(QtCharts::QChart::ChartThemeBlueCerulean);
    //添加动画
    m_Chart->setAnimationOptions(QChart::SeriesAnimations);
    //添加图表标题
    m_Chart->setTitle(QString::fromUtf8("PWN电压变化值"));
    //将图表绑定到视图
    ui->chartWidget1->setChart(m_Chart);
    //设置反走样(平滑曲线) 去锯齿
    ui->chartWidget1->setRenderHint(QPainter::Antialiasing,true);
    m_lineSeries->setUseOpenGL(true);
}
