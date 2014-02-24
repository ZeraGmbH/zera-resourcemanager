#ifndef H2012_CLIENT_H
#define H2012_CLIENT_H

#include <QObject>
#include <QQueue>

#include <netmessages.pb.h>

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
     * @brief getName
     * @return Name of the client
     */
    const QString& getName();

    /**
     * @brief getIpAdress
     * @return
     */
    QString getIpAdress();

  signals:
    /**
     * @brief The client disconnected from the server
     */
    void aboutToDisconnect();

    /**
      @brief Notifies the SCPI::SCPIInterface of new SCPI commands
      */
    void scpiCommandSent(const ProtobufMessage::NetMessage::ScpiCommand &command);

  public slots:
    /**
     * @brief Sends acknowledgement
     * @param message Optional text
     */
    void sendACK(const QString &message=QString());
    /**
     * @brief Sends debug informations
     * @param message Required text
     */
    void sendDebug(const QString &message);
    /**
     * @brief Sends an error message
     * @param message Optional text
     */
    void sendError(const QString &message=QString());
    /**
     * @brief Sends negative acknowledgement
     * @param message Optional text
     */
    void sendNACK(const QString &message=QString());
    /**
     * @brief Decodes incoming messages into a ProtobufMessage
     * @param message Unparsed message
     */
    void messageReceived(google::protobuf::Message *message);

  private:
    /**
     * @brief The Client representated
     */
    ProtoNetPeer* m_zClient;

    /**
     * @brief Internal code to send a ProtobufMessage to the client
     * @param message a ProtobufMessage
     */
    void sendMessage(ProtobufMessage::NetMessage *envelope);

    /// @todo PIMPL
    QQueue<std::string> clientIdQueue;
    QQueue<qint64> messageIdQueue;
    QString m_name;
  };
}

#endif // H2012_CLIENT_H
