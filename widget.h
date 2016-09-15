#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class QUdpSocket;
class TcpServer;
class QTextCharFormat;
enum MessageType {Message,NewParticipant,ParticipantLeft,FileName,Refuse};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void newParticipant(QString userName,
                        QString localHostName,QString ipAddress);
    void participantLeft(QString username,
                         QString localHostName,QString time);
    void sendMessage(MessageType type,QString serverAddress = "");

    void hasPendingFile(QString userName, QString serverAddress,
                        QString clientAddress,QString fileName);
    bool saveFile(const QString &fileName);

    void closeEvent(QCloseEvent *event);

    QString getIP();
    QString getUserName();
    QString getMessage();
private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString fileName;
    TcpServer *server;
    QColor color;

private slots:
    void processPendingDatagrams();
    void on_sendButton_clicked();
    void getFileName(QString);
    void on_sendToolBtn_clicked();
    void on_SizeComboBox_currentIndexChanged(const QString &arg1);
    void on_boldToolBtn_clicked(bool checked);
    void on_italicToolBtn_clicked(bool checked);
    void on_underlineToolBtn_clicked(bool checked);
    void on_colorToolBtn_clicked(bool checked);
    void currentFormatChanged(const QTextCharFormat &format);
    void on_saveToolBtn_clicked();
    void on_clearToolBtn_clicked();
    void on_exitButton_clicked();
};

#endif // WIDGET_H
