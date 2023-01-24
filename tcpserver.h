#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class TCPServer : public QObject {
    Q_OBJECT
  public:
    explicit TCPServer (QObject *parent = nullptr);

  public slots:
    void writeData();
    void makeCANConnection();
    void readTcpData();

  private:
    int port = 36468;
    QString remoteAddress = "";

    QTcpServer *m_Server;
    QTcpSocket *m_Client;
  signals:

};

#endif // TCPSERVER_H
