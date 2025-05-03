#include "mcpclient.h"
#include <QHostAddress>

MCPClient::MCPClient(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &MCPClient::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, &MCPClient::onError);
}

void MCPClient::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

void MCPClient::sendMessage(const QByteArray &message)
{
    m_socket->write(message);
}

void MCPClient::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    emit messageReceived(data);
}

void MCPClient::onError(QAbstractSocket::SocketError socketError)
{
    emit errorOccurred(m_socket->errorString());
}