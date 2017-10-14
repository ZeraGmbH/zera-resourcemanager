#ifndef CLIENTMULTITON_H
#define CLIENTMULTITON_H

#include <QObject>
#include <QSet>
#include <netmessages.pb.h>

namespace Application
{
  class Resource;
}

namespace ResourceServer
{
  class ClientSocket;

  /**
   * @brief Multiple clients can share one socket connection
   * this class represents a client
   */
  class ClientMultiton : public QObject
  {
    Q_OBJECT
  public:
    explicit ClientMultiton(ResourceServer::ClientSocket *t_parent, const QByteArray& t_clientId);
    ~ClientMultiton();
    QString getName() const;

    QString getIpAddress() const;
  signals:
    /**
     * @brief is called when a client sent the IDENT command
     */
    void sigClientIdentified();
    /**
      *@brief Notifies the SCPI::SCPIInterface of new SCPI commands
      */
    void sigScpiTransaction(ClientMultiton *t_clientMultiton, const ProtobufMessage::NetMessage_ScpiCommand &t_command);

  public slots:
    /**
      * @brief Sends acknowledgement
      * @param message Optional text
      */
    void doSendACK(const QString &t_message=QString()) const;
    /**
      * @brief Sends debug informations
      * @param message Required text
      */
    void doSendDebug(const QString &t_message) const;
    /**
      * @brief Sends an error message
      * @param message Optional text
      */
    void doSendError(const QString &t_message=QString()) const;
    /**
   * @brief Sends negative acknowledgement
   * @param message Optional text
   */
    void doSendNACK(const QString &t_message=QString()) const;
    /**
      * @brief Decodes incoming messages into a ProtobufMessage
      * @param message Unparsed message
      */
    void onMessageReceived(ProtobufMessage::NetMessage *t_envelope);

  private:
    QString m_name;

    ResourceServer::ClientSocket *m_parent;
    const QByteArray m_clientId;

    Q_DISABLE_COPY(ClientMultiton)
  };
}
#endif // CLIENTMULTITON_H
