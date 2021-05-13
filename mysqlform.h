#ifndef MYSQLFORM_H
#define MYSQLFORM_H

#include <QWidget>
#include <QMenu>
#include <QAction>


namespace Ui
{
class MysqlForm;
}



class MysqlForm : public QWidget
{
    Q_OBJECT
    friend void addDataToLibrary(QString time,QString issue,QString type);

public:
    explicit MysqlForm(QWidget *parent = nullptr);
    ~MysqlForm();
    void initMysqlData();
    static void addDataToLibrary(QString time,QString issue,QString type);

private:
    Ui::MysqlForm *ui;
    QMenu* rightMenu;
    QAction* refresh,*deleteData,*deleteAll;
    QString fliter,flitername;
    int btn_count=0;

private:
    void RefreshData();

private slots:
    void Process_Refresh();
    void Process_Delete();
    void Process_DeleteAll();
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void on_btn_fliter_clicked();
};

#endif // MYSQLFORM_H
