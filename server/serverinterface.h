#ifndef H2012_SERVERINTERFACE_H
#define H2012_SERVERINTERFACE_H


#include <QObject>
#include <QList>

class ProtoNetServer;
class ProtoNetPeer;

class RMProtobufWrapper;

namespace Application
{
  class Resource;
}

/**
  @brief The Server Namespace encapsulates all networking logic into a separate namespace.
 */
namespace Server
{
  class Client;
  /**
    @brief Represents the interface between the network implementations and the ResourceManager
    */
  class ServerInterface : public QObject
  {
    Q_OBJECT
  public:
    /**
      @brief See [P.157+ Design patterns Gang of Four]
      */
    static ServerInterface* getInstance();

  public slots:
    /**
      @brief Will be triggered when the Server::Client disconnects
      */
    void clientDisconnected();

    /**
     * @brief Is called when new clients arrive
     * @param [in] *zcl the Zera::Net::cClient that just connected
     */
    void newClient(ProtoNetPeer *zcl);

  protected:
    /**
      @brief The class is a Singleton so the constructor is protected [P.157+ Design patterns Gang of Four]
      */
    ServerInterface(QObject* parent = 0);
    ~ServerInterface();
    /**
      @brief See [P.157+ Design patterns Gang of Four]
      */
    static ServerInterface* singletonInstance;

  private:
    /**
      @brief  list of all Server::Client instances this server handles
      */
    QList<Client*> m_clients;

    /**
     * @brief Internally used server
     */
    ProtoNetServer* m_zServer;

    RMProtobufWrapper* m_defaultWrapper;

    /**
      @note Instances of this class should only get accessed through the getInstance method.
      */
    Q_DISABLE_COPY(ServerInterface)
  };
}
#endif // H2012_SERVERINTERFACE_H
