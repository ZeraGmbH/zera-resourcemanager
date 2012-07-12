#ifndef H2012_SERVERINTERFACE_H
#define H2012_SERVERINTERFACE_H

#include <QObject>
#include "server/client.h"

namespace Resource
{
  class ResourceObject;
  class Detail;
}

/**
  @brief The Server Namespace encapsulates all networking logic into a separate namespace.
 */
namespace Server
{
  /**
    @b Represents the interface between the network implementations and the ResourceManager
    */
  class ServerInterface : public QObject
  {
    Q_OBJECT
  protected:
    /**
      @b The class is a Singleton so the constructor is protected [P.157+ Design patterns Gang of Four]
      */
    ServerInterface(QObject* parent = 0);

  public:
    /**
      @b See [P.157+ Design patterns Gang of Four]
      */
    static ServerInterface* getInstance();

  signals:
    /**
      @b Will be triggered when a Server::Client provides a new Resource::ResourceObject
      @param [in] *resource the Resource::ResourceObject that will be added in ResourceManager
      @param [in] *client the Server::Client that requested the operation
      */
    void newResource(Resource::ResourceObject* resource, Client* client);

    /**
      @b Will be triggered when a Server::Client wants to delete a Resource::ResourceObject
      @param [in] *resource the Resource::ResourceObject that will be  deleted in ResourceManager
      @param [in] *client the Server::Client that requested the operation
      */
    void deleteResource(Resource::ResourceObject* resource, Client* client);

    /**
      @b Will be triggered when a Server::Client wants to lock a Resource::ResourceObject
      @param [in] *resource the Resource::ResourceObject that will be locked with Resource::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void lockResource(Resource::ResourceObject* resource, Client* client);

    /**
      @b Will be triggered when a Server::Client wants to unlock Resource::ResourceObject
      @param [in] *resource the Resource::ResourceObject that will be unlocked with Resource::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void unlockResource(Resource::ResourceObject* resource, Client* client);

  public slots:
    /**
      @b Will be triggered when a Client connects
      @param [in] *client the Server::Client that requested the operation
      */
    void newClient(Client* client);

    /**
      @b Will be triggered when the Server::Client disconnects
      @param [in] *client the Server::Client that requested the operation
      */
    void disconnected(Client* client);

    /**
      @b Will be triggered when a Client wants to lock a Resource:ResourceObject but it is already locked
      @param [in] *resource the Resource::ResourceObject that will be locked with Resource::ResourceLock
      @param [in] *client the Server::Client that requested the operation
      */
    void lockFailed(Resource::ResourceObject* resource, Server::Client* client);

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
      @note Instances of this class should only get accessed through the getInstance method.
      */
    Q_DISABLE_COPY(ServerInterface)
  };
}
#endif // H2012_SERVERINTERFACE_H
