#ifndef STM32MYSQL_H
#define STM32MYSQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSql>

namespace stm32Mysql
{
    void initMysql();
    QString getOpenMysqlError();
    bool getConnectStatus();
}


using namespace stm32Mysql;
class STM32MySQL
{
public:
    STM32MySQL();
    void connectToMysql();
    static QSqlDatabase mydb;
};

#endif // STM32MYSQL_H
