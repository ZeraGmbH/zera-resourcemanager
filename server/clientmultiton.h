#ifndef CLIENTMULTITON_H
#define CLIENTMULTITON_H

#include <QObject>
#include <QHash>
#include <QSet>
#include <netmessages.pb.h>

namespace Application
{
  class Resource;
}

namespace google
{
  namespace protobuf
  {
    class Message;
  }
}

namespace Server
{
  class Client;

  class ClientMultiton : public QObject
  {
    Q_OBJECT
  public:
    explicit ClientMultiton(Server::Client *parent, const QByteArray& cID);

    QString name() const;


    void addOccupation(Application::Resource *res);
    void removeOccupation(Application::Resource *res);

    QString getIpAddress();
    const QByteArray &getClientID();
  signals:
    /**
      *@brief Notifies the SCPI::SCPIInterface of new SCPI commands
      */
    void sigScpiTransaction(const ProtobufMessage::NetMessage::ScpiCommand &command);

  public slots:
    /**
      * @brief Sends acknowledgement
      * @param message Optional text
      */
    void doSendACK(const QString &message=QString());
    /**
      * @brief Sends debug informations
      * @param message Required text
      */
    void doSendDebug(const QString &message);
    /**
      * @brief Sends an error message
      * @param message Optional text
      */
    void doSendError(const QString &message=QString());
    /**
   * @brief Sends negative acknowledgement
   * @param message Optional text
   */
    void doSendNACK(const QString &message=QString());
    /**
      * @brief Decodes incoming messages into a ProtobufMessage
      * @param message Unparsed message
      */
    void onMessageReceived(ProtobufMessage::NetMessage *envelope);

    /**
      * @brief cleanup all occupied resources
      */
    void onDisconnectCleanup();

  private:
    QSet<Application::Resource *> m_occupies;
    QSet<Application::Resource *> m_provides;
    QString m_name;

    Server::Client *m_parent;
    const QByteArray m_clientId;

    Q_DISABLE_COPY(ClientMultiton)
  };

}
#endif // CLIENTMULTITON_H
