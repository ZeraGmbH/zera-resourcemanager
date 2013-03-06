#ifndef H2012_SERVERINTERFACE_H
#define H2012_SERVERINTERFACE_H

#include <QTcpServer>
#include <QThread>
#include "server/client.h"

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
  class ServerInterface : public QTcpServer
  {
    Q_OBJECT
  protected:
    /**
      @b The class is a Singleton so the constructor is protected [P.157+ Design patterns Gang of Four]
      */
    ServerInterface(QObject* parent = 0);
    ~ServerInterface();
    /**
      @b Overload the incoming connection to creater our own threaded Server::Client
      */
    void incomingConnection(int socketDescriptor);

  public:
    /**
      @b See [P.157+ Design patterns Gang of Four]
      */
    static ServerInterface* getInstance();

  signals:
    /**
      @b Will be triggered when a Server::Client provides a new Application::ResourceObject
      @param [in] *resource the Application::ResourceObject that will be added in ResourceManager
      @param [in] *client the Server::Client that requested the operation
      */
    void newResource(Application::Resource* resource, Client* client);

    /**
      @b Will be triggered when a Server::Client wants to delete a Application::ResourceObject
      @param [in] *resource the Application::ResourceObject that will be  deleted in ResourceManager
      @param [in] *client the Server::Client that requested the operation
      */
    void deleteResource(Application::Resource* resource, Client* client);

    /**
      @b Will be triggered when a Server::Client wants to lock a Application::ResourceObject
      @param [in] *resource the Application::ResourceObject that will be locked with Application::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void lockResource(Application::Resource* resource, Client* client);

    /**
      @b Will be triggered when a Server::Client wants to unlock Application::ResourceObject
      @param [in] *resource the Application::ResourceObject that will be unlocked with Application::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void unlockResource(Application::Resource* resource, Client* client);

  public slots:
    /**
      @b Will be triggered when the Server::Client disconnects
      */
    void clientDisconnect();
    /**
      @b Will be triggered when a Client wants to lock a Resource:ResourceObject but it is already locked
      @param [in] *resource the Application::ResourceObject that will be locked with Application::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void lockFailed(Application::Resource* resource, Server::Client* client);

    /**
      @b Will be triggered when a Client wants to lock a Resource:ResourceObject and succeeds
      @param [in] *resource the Application::ResourceObject that will be locked with Application::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void lockGranted(Application::Resource* resource, Server::Client* client);

  private:
    /**
      @b See [P.157+ Design patterns Gang of Four]
      */
    static ServerInterface* singletonInstance;
    /**
      @b  list of all Server::Client instances this server handles
      */
    QList<Client*> clients;

    /**
      @b The thread where the clients live in
      */
    QThread* netThread;

    /**
      @note Instances of this class should only get accessed through the getInstance method.
      */
    Q_DISABLE_COPY(ServerInterface)
  };
}
#endif // H2012_SERVERINTERFACE_H
