#include "orderthread.h"
#include <QDebug>


OrderThread::OrderThread(QObject* parent) : QThread(parent)
{
    //parent->writeOrderToSerialPort(order);
}



OrderThread::~OrderThread()
{
    this->quit();
}



void OrderThread::run()
{
    while (m_bStart) {
        Task * task = popTask();
        if(task){
            task->sendData();
            emit sendUartOrder(task->getOrder(),task->getID());
            delete task;
            task = nullptr;
        }
    }
    msleep(100);
}



//添加任务,在最前面添加,保证最后添加的优先级最高
void OrderThread::addTask(Task *task)
{
    m_mutex.lock();
    m_taskList.push_front(task);
    m_mutex.unlock();
}



Task *OrderThread::popTask()
{
    m_mutex.lock();
    Task * task;
    if(m_taskList.size() == 0){
        task =  nullptr;
    }
    else{
        task = m_taskList.takeFirst();
    }
    m_mutex.unlock();
    return task;
}



Task::Task(QString order,QString id):m_order(order),m_id(id)
{

}



void Task::sendData()
{
    //要做的任务
}
