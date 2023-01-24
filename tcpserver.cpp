#include "tcpserver.h"
#include<QDebug>
#include <QTimer>

TCPServer::TCPServer (QObject *parent)
  : QObject{parent} {
  m_Server = new QTcpServer (this);
  remoteAddress = "127.0.0.1";//"192.168.56.1";
  connect (m_Server, &QTcpServer::newConnection, this, &TCPServer::makeCANConnection);
  m_Server->listen (QHostAddress (remoteAddress), port);
  qDebug() << "Server started" << m_Server->isListening() << m_Server->serverPort();
}

void TCPServer::makeCANConnection() {
  qDebug() << "New connection";
  m_Client = m_Server->nextPendingConnection();
  connect (m_Client, &QTcpSocket::readyRead, this, &TCPServer::readTcpData);
  QTimer *timer = new QTimer;
  connect (timer, &QTimer::timeout, this, &TCPServer::writeData);
  timer->start (100);
}


void TCPServer::readTcpData() {
  QByteArray data = m_Client->readLine();
  qDebug() << data;
}

/* @brief
* Catch socketcand broadcast beacon
* Extract socket port and can interfaces
*/
void TCPServer::writeData() {
  if (!m_Client->isOpen())
    return;
  QByteArray sendframe;
  sendframe.append (0x43);
  const uint32_t frameId = 0x18FEDA31;
  const uint8_t byteMSB = frameId % 256;
  const uint8_t byteMSB1 = (frameId / 256) % 256;
  const uint8_t byteMSB2 = (frameId / (256 * 256)) % 256;
  const uint8_t byteLSB = (frameId / (256 * 256 * 256)) % 256;
  sendframe.append (byteMSB);
  sendframe.append (byteMSB1);
  sendframe.append (byteMSB2);
  sendframe.append (byteLSB);
  sendframe.append (0x02);
  sendframe.append (8);
  QByteArray tmpData = QByteArray::fromHex ("1122334455667788");
  sendframe.append (tmpData);
  qDebug() << "Written frame" << m_Client->peerAddress() << m_Client->write (sendframe);
}

