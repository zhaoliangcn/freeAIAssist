#ifndef MCPCLIENT_H
#define MCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class MCPClient : public QObject
{
    Q_OBJECT

public:
    explicit MCPClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QByteArray &message);

signals:
    void messageReceived(const QByteArray &message);
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;
};

#endif // MCPCLIENT_H