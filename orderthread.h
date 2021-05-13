#ifndef ORDERTHREAD_H
#define ORDERTHREAD_H

#include "usartform.h"
#include <QThread>
#include <QObject>
#include <QMutex>
#include <QDebug>


//参数:
//order:线程要发送给stm32的指令



//单个任务类
class Task
{
public:
    Task(QString order,QString id);
    void sendData();
    QString getOrder(){return m_order;}
    QString getID(){return m_id;}
private:
    QString m_order,m_id;
};
class OrderThread : public QThread
{
    Q_OBJECT
public:
    OrderThread(QObject* parent=nullptr);
    ~OrderThread();
    void run();
    void addTask(Task * task); //添加任务,在最前面添加,保证最后添加的优先级最高
    Task * popTask();
    void stop(){m_bStart = false;}
    bool isListEmpty()
    {
        qDebug()<<m_taskList.count();
        if(m_taskList.count()<1)
            return true;
        return false;
    }
private:
    QList<Task *> m_taskList;  //任务列表
    QMutex m_mutex;
    bool m_bStart = true;

signals:
    void sendUartOrder(QString order,QString id); //图片保存完成
};

#endif // ORDERTHREAD_H
