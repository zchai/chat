#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>

namespace Ui {
class TcpClient;
}

class TcpClient : public QDialog
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

private:
    Ui::TcpClient *ui;
};

#endif // TCPCLIENT_H
