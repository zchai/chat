#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QTcpSocket>
#include <QMessageBox>

TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setFixedSize(350,180);

    TotalBytes = 0;
    bytesReceive = 0;
    fileNameSize = 0;
    tcpClient = new QTcpSocket(this);
    tcpPort = 6666;

    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::setHostAddress(QHostAddress address)
{
    hostAddress = address;
    newConnect();
}

void TcpClient::setFileName(QString fileName)
{
    localFile = new QFile(fileName);
}

void TcpClient::closeEvent(QCloseEvent *)
{
    on_tcpClientCloseBtn_clicked();
}

void TcpClient::newConnect()
{
    blockSize = 0;
    tcpClient->abort();
    tcpClient->connectToHost(hostAddress,tcpPort);
    time.start();
}

void TcpClient::readMessage()
{
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_5_4);

    float useTime = time.elapsed();
    qDebug() << bytesReceive << endl;
    if(bytesReceive <= sizeof(qint64)*2) {
        if((tcpClient->bytesAvailable()>=sizeof(qint64) * 2) && (fileNameSize == 0))
        {
            in >> TotalBytes >> fileNameSize;
            bytesReceive += sizeof(qint64) * 2;
        }
        if((tcpClient->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)) {
            in >> fileName;
            bytesReceive += fileNameSize;
            if(!localFile->open(QFile::WriteOnly)) {
                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1：\n %2")
                                     .arg(fileName).arg(localFile->errorString()));
                return;
            }

        }
        else
        {
            return;
        }
    }
    if(bytesToReceive < TotalBytes) {
        bytesReceive += tcpClient->bytesAvailable();
        inBlock = tcpClient->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesReceive);
    double speed = bytesReceive / useTime;
    ui->tcpClientStatuslabel->setText(tr("已接收 %1 MB (%2MB/s)"
                                         "\n共%3MB.已用时：%4秒\n估计剩余时间：%5秒")
                                      .arg(bytesReceive / (1024*1024))
                                      .arg(speed * 1000/(1024 * 1024),0,'f',2)
                                      .arg(TotalBytes / (1024*1024))
                                      .arg(useTime/1000,0,'f',0)
                                      .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0));
    if(bytesReceive == TotalBytes) {
        localFile->close();
        tcpClient->close();
        ui->tcpClientStatuslabel->setText(tr("接收文件%1完毕").arg(fileName));
    }
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError: break;
    default :
        qDebug() << tcpClient->errorString();
    }
}

void TcpClient::on_tcpClientBtn_clicked()
{
    tcpClient->abort();
    if(localFile->isOpen())
        localFile->close();
}

void TcpClient::on_tcpClientCloseBtn_clicked()
{
    tcpClient->abort();
    if(localFile->isOpen())
        localFile->close();
    close();
}

