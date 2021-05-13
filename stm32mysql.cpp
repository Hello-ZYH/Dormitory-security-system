#include "stm32mysql.h"



QSqlDatabase STM32MySQL::mydb;
STM32MySQL::STM32MySQL()
{
    stm32Mysql::initMysql();
}



//创建数据库
void stm32Mysql::initMysql()
{
    qDebug()<<"starting init mysql!";
    qDebug()<<QSqlDatabase::drivers();

    STM32MySQL::mydb=QSqlDatabase::addDatabase("QODBC");   //数据库驱动类型为SQL Server
    STM32MySQL::mydb.setHostName("localhost");             //选择本地主机，127.0.1.1
    STM32MySQL::mydb.setDatabaseName("stm32f103");         //设置数据源名称
    STM32MySQL::mydb.setUserName("Stm32");                 //登录用户
    STM32MySQL::mydb.setPassword("123456");                //密码
    STM32MySQL::mydb.setPort(1433);
    STM32MySQL::mydb.open();
}



//打开数据库失败提示
QString stm32Mysql::getOpenMysqlError()
{
    return STM32MySQL::mydb.lastError().text();
}



//获取连接状态
bool stm32Mysql::getConnectStatus()
{
    return STM32MySQL::mydb.isOpen();
}
