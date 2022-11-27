#ifndef H2012_CLIENT_H
#define H2012_CLIENT_H

#include <QObject>
#include <QQueue>
#include <QHash>

#include <memory>

class XiQNetPeer;

namespace google
{
namespace protobuf
{
class Message;
}
}

namespace ProtobufMessage
{
class NetMessage;
}

namespace ResourceServer
{
class ClientMultiton;
/**
  @brief Server::Client represents an abstract client implementation
  @todo This class needs unique identifiers for implementation clients.
  */
class ClientSocket : public QObject
{
    Q_OBJECT
public:
    /**
      @brief The default constructor
      @note Other constructors are invalid
      */
    explicit ClientSocket(XiQNetPeer *t_clientSocket, QObject *parent = 0);
    ~ClientSocket();

    /**
     * @brief getIpAdress
     * @return
     */
    QString getIpAdress() const;

signals:
    /**
     * @brief The client disconnected from the server
     */
    void sigAboutToDisconnect();

    /**
     * @brief Used by shared connections
     */
    void sigDisconnectedClientId(const QByteArray &t_clientId);

    /**
     * @brief is called when a ClientMultiton received the IDENT command
     * @param t_clientMultiton
     */
    void sigClientIdentified(ClientMultiton *t_clientMultiton);

    /**
     * @brief is called when the ClientMultiton is disconnected
     * @param t_clientMultiton
     */
    void sigClientMultitonDisconnected(ClientMultiton *t_clientMultiton);

public slots:
    /**
     * @brief Sends acknowledgement
     * @param message Optional text
     */
    void doSendACK(const QString &t_message=QString(), const QByteArray &t_cID=QByteArray()) const;
    /**
     * @brief Sends debug informations
     * @param message Required text
     */
    void doSendDebug(const QString &t_message, const QByteArray &t_cID=QByteArray()) const;
    /**
     * @brief Sends an error message
     * @param message Optional text
     */
    void doSendError(const QString &t_message=QString(), const QByteArray &t_cID=QByteArray()) const;
    /**
     * @brief Sends negative acknowledgement
     * @param message Optional text
     */
    void doSendNACK(const QString &t_message=QString(), const QByteArray &t_cID=QByteArray()) const;
    /**
     * @brief Decodes incoming messages into a ProtobufMessage
     * @param message Unparsed message
     */
    void onMessageReceived(std::shared_ptr<google::protobuf::Message> t_message);

private slots:
    /**
     * @brief cleanup all occupied resources
     */
    void onDisconnectCleanup();

private:
    /**
     * @brief Internal code to send a ProtobufMessage to the client
     * @param message a ProtobufMessage
     */
    void sendMessage(ProtobufMessage::NetMessage &t_message) const;

    /**
     * @brief The Client representated
     */
    XiQNetPeer* m_zClient;
    QQueue<qint64> m_messageIdQueue;

    //QSet<QByteArray> m_clientIds;

    QHash<QByteArray, ClientMultiton*> m_clientSockets;
    Q_DISABLE_COPY(ClientSocket)
};
}

#endif // H2012_CLIENT_H
