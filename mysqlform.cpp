#include "mysqlform.h"
#include "ui_mysqlform.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QTextBrowser>



MysqlForm::MysqlForm(QWidget *parent) : QWidget(parent), ui(new Ui::MysqlForm)
{
    ui->setupUi(this);
    fliter=QString("where operate_type='%1'");
    flitername="串口";
}



MysqlForm::~MysqlForm()
{
    delete ui;
}



//添加数据到数据库
void MysqlForm::addDataToLibrary(QString time, QString thing, QString operate)
{
    QSqlQuery query;
    QString sql_str = QString("insert into data values(1,'%1','%2','%3')").arg(time,thing,operate);
    query.exec(sql_str);
    //RefreshData();
}



void MysqlForm::initMysqlData()
{
    //鼠标右键操作
    rightMenu = new QMenu(this);
    refresh = new QAction(QString("刷新数据"),rightMenu);
    deleteData = new QAction(QString("删除一条"),rightMenu);
    deleteAll = new QAction(QString("删除全部"),rightMenu);

    rightMenu->addAction(refresh);
    rightMenu->addAction(deleteData);
    rightMenu->addAction(deleteAll);

    connect(refresh,&QAction::triggered,this,&MysqlForm::Process_Refresh);
    connect(deleteData,&QAction::triggered,this,&MysqlForm::Process_Delete);
    connect(deleteAll,&QAction::triggered,this,&MysqlForm::Process_DeleteAll);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    RefreshData();
}


//刷新数据按钮
void MysqlForm::RefreshData()
{
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem(QIcon(":/image/images/down.png"),"操作类型"));

    QSqlQuery query;
    QString sql_str;
    QStringList headers;

    headers<<"序号"<<"时间"<<"事件"<<"操作类型";
    sql_str = QString("select count(*) as count from data");//+QString(" "+fliter.arg(flitername));
    query.exec(sql_str);
    query.next();

    int rowCount = query.value("count").toInt();
    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setColumnCount(headers.count());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    sql_str = QString("select * from data");              // +QString(" "+fliter.arg(flitername));
    query.exec(sql_str);

    int i=0;
    while(query.next())
    {
        QTextBrowser* issue = new QTextBrowser();
        issue->append(query.value("thing").toString());

        ui->tableWidget->setCellWidget(i,0,new QLabel(query.value("number").toString()));
        ui->tableWidget->setCellWidget(i,1,new QLabel(query.value("time").toString()));
        ui->tableWidget->setCellWidget(i,2,issue);
        ui->tableWidget->setCellWidget(i,3,new QLabel(query.value("operate").toString()));
        i++;
    }
    // ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->hideColumn(0);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(2,200);
}



//刷新数据
void MysqlForm::Process_Refresh()
{
    RefreshData();
    qDebug()<<"刷新成功!";
}



//删除单行数据
void MysqlForm::Process_Delete()
{
    QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();
    if(ranges.count()<1)
    {
        QMessageBox::warning(this,"tips","请选中一行!(单次只能删除一行)");
        return;
    }
    int id = ((QLabel*)ui->tableWidget->cellWidget(ranges.at(0).topRow(),0))->text().toInt();
    QSqlQuery query;
    query.exec(QString("delete from data where number=%1").arg(id));
    RefreshData();
}



//删除全部数据
void MysqlForm::Process_DeleteAll()
{
    QSqlQuery query;
    query.exec(QString("truncate table data"));
    RefreshData();
}



void MysqlForm::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    rightMenu->show();
    rightMenu->exec(QCursor::pos());
}



//切换wifi和串口
void MysqlForm::on_btn_fliter_clicked()
{
    btn_count++;
    if(btn_count%2!=0)
    {
        flitername="wifi";
        ui->btn_fliter->setIcon(QIcon(":/image/images/wifi.png"));
        ui->label_type->setText("wifi");
        RefreshData();
    }
    else
    {
        flitername="串口";
        ui->btn_fliter->setIcon(QIcon(":/image/images/uart.png"));
        ui->label_type->setText("串口");
        RefreshData();
    }
}
