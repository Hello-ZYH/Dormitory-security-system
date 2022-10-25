//#ifndef PICTUREFORM_H
//#define PICTUREFORM_H

//#include <QDialog>
//#include <QWidget>
//#include <QAbstractSocket>
//#include <QTcpServer>


//class QTcpSocket;
//class QFile;

//namespace Ui {
//class PictureForm;
//}

//class PictureForm : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit PictureForm(QWidget *parent = nullptr);
//    ~PictureForm();

//public slots:
//    void updateServerProgress();//接收并处理显示图片
//    void start();//监听的事件
//    void acceptConnection();//被客户端连接上后，创建套接字、接收数据、处理异常、关闭服务器

//  //void displayError(QAbstractSocket::SocketError socketError);//错误处理

//    //base64字符串转图片
//    QImage getImage(const QString &);

//private:
//    Ui::PictureForm *ui;

//    QTcpServer tcpServer;
//    QTcpSocket *tcpServerConnection;
//    qint64 totalBytes;     // 存放总大小信息
//    qint64 bytesReceived;  // 已收到数据的大小
//    qint64 fileNameSize;   // 文件名的大小信息
//    qint64 imageSize; //图片大小

//    QString fileName;      // 存放文件名
//    QFile *localFile;      // 本地文件
//    QByteArray inBlock;    // 数据缓冲区
//    QString imageContent;

//    QImage image;//图片


//private slots:

//    //void on_startButton_clicked();//监听或断开监听
//};

//#endif // PICTUREFORM_H
