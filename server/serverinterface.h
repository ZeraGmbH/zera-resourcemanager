#ifndef H2012_SERVERINTERFACE_H
#define H2012_SERVERINTERFACE_H


#include <QObject>
#include <QList>

#include "server/client.h"
#include <zeraserver.h>

namespace Application
{
  class Resource;
}


/**
  @brief The Server Namespace encapsulates all networking logic into a separate namespace.
 */
namespace Server
{
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

  signals:
    /**
      @brief Will be triggered when a Server::Client provides a new SCPI::ResourceObject
      @param [in] *resource the SCPI::ResourceObject that will be added in ResourceManager
      @param [in] *client the Server::Client that requested the operation
      */
    void newResource(Application::Resource* resource, Client* client);

    /**
      @brief Will be triggered when a Server::Client wants to delete a SCPI::ResourceObject
      @param [in] *resource the SCPI::ResourceObject that will be  deleted in ResourceManager
      @param [in] *client the Server::Client that requested the operation
      */
    void deleteResource(Application::Resource* resource, Client* client);

  public slots:
    /**
      @brief Will be triggered when the Server::Client disconnects
      */
    void clientDisconnected();

    /**
     * @brief Is called when new clients arrive
     * @param [in] *cl the Zera::Net::ZeraClient that just connected
     */
    void newClient(Zera::Net::ZeraClient *zcl);

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
    QList<Client*> clients;

    /**
     * @brief Internally used server
     */
    Zera::Net::ZeraServer* m_zServer;

    /**
      @note Instances of this class should only get accessed through the getInstance method.
      */
    Q_DISABLE_COPY(ServerInterface)
  };
}
#endif // H2012_SERVERINTERFACE_H
