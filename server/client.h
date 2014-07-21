#ifndef H2012_CLIENT_H
#define H2012_CLIENT_H

#include <QObject>
#include <QQueue>
#include <QSet>

#include <netmessages.pb.h>

#include "resource/resource.h"

class ProtoNetPeer;

namespace Server
{

  /**
    @brief Server::Client represents an abstract client implementation, with a timeout/refresh function
    @todo This class needs unique identifiers for implementation clients.
    */
  class Client : public QObject
  {
    Q_OBJECT
  public:
    /**
      @brief The default constructor
      @note Other constructors are invalid
      */
    explicit Client(ProtoNetPeer* zClient, QObject *parent = 0);

    /**
     * @brief getIpAdress
     * @return
     */
    QString getIpAdress();

  signals:
    /**
     * @brief The client disconnected from the server
     */
    void sigAboutToDisconnect();

    /**
     * @brief Used by shared connections
     */
    void sigDisconnectedClientId(const QByteArray& clientId);

  public slots:
    /**
     * @brief Sends acknowledgement
     * @param message Optional text
     */
    void doSendACK(const QString &message=QString(), const QByteArray &cID=QByteArray());
    /**
     * @brief Sends debug informations
     * @param message Required text
     */
    void doSendDebug(const QString &message, const QByteArray &cID=QByteArray());
    /**
     * @brief Sends an error message
     * @param message Optional text
     */
    void doSendError(const QString &message=QString(), const QByteArray &cID=QByteArray());
    /**
     * @brief Sends negative acknowledgement
     * @param message Optional text
     */
    void doSendNACK(const QString &message=QString(), const QByteArray &cID=QByteArray());
    /**
     * @brief Decodes incoming messages into a ProtobufMessage
     * @param message Unparsed message
     */
    void onMessageReceived(google::protobuf::Message *message);

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
    void sendMessage(ProtobufMessage::NetMessage *envelope);

    /**
     * @brief The Client representated
     */
    ProtoNetPeer* m_zClient;
    QQueue<qint64> m_messageIdQueue;

    //QSet<QByteArray> m_clientIds;

    QHash<QByteArray, ClientMultiton*> m_clients;
  };
}

#endif // H2012_CLIENT_H
