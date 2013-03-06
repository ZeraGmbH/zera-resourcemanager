#ifndef H2012_RESOURCEMANAGER_H
#define H2012_RESOURCEMANAGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QTextStream>
#include "server/client.h"
#include "server/serverinterface.h"
#include "resource/resource.h"

/**
  @brief Represents the managing instance in this application that holds Application::ResourceObjects and Application::ResourceLock
  */
class ResourceManager : public QObject
{
  Q_OBJECT
protected:
  /**
    @b The class is a Singleton so the constructor is protected [P.157+ Design patterns Gang of Four]
    */
  ResourceManager(QObject* parent = 0);

public:
  /**
    @b See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* getInstance();

  /**
    @b Lists all resources of a given type as QString
    @returns QString with resources separated by a semicolon
    */
  const QString &listResources(const QString &Type);

  /**
    @b Creates a Application::Resource and appends it to the resourceList
    */
  Application::Resource *createResource(quint32 amount, const QString &description, const QString &name, int provider, const QString &type);


public slots:
  /**
    @b Will be triggered when the a Server::Client provides a new Resource:ResourceObject
    @param [in] *resource the Application::ResourceObject that will be added in ResourceManager
    */
  void newResource(Application::Resource* resource);

  /**
    @b Will be triggered when the a Server::Client wants to delete a Resource:ResourceObject
    @param [in] *resource the Application::ResourceObject that will be  deleted in ResourceManager
    */
  void deleteResource(Application::Resource* resource);

  /**
    @b Will be triggered when the a Server::Client wants to lock a Resource:ResourceObject
    @param [in] *resource the Application::ResourceObject that will be locked with Application::ResourceLock
    @param [in] *client the Server::Client that attempts to lock the Application::ResourceObject
    @param amount if the resource has a quantity the amount of the resource that should be reserved can be specified
    */
  void occupyResource(Application::Resource* resource, Server::Client* client, quint32 amount=0);

  /**
    @b Will be triggered when the a Server::Client wants to unlock Resource:ResourceObject
    @param [in] *resource the Application::ResourceObject that will be unlocked with Application::ResourceLock
    @param [in] *client the Server::Client requesting the operation
    @param amount if the resource has a quantity the amount of the resource that should be freed can be specified
    */
  void freeResource(Application::Resource* resource, Server::Client* client, quint32 amount=0);

private:
  /**
    @b See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* singletonInstance;

  QReadWriteLock rwLock;

  QList<Application::Resource*> resourceList;
  /**
    @note Instances of this class should only get accessed through the getInstance method.
    */
  Q_DISABLE_COPY(ResourceManager)
};

#endif // H2012_RESOURCEMANAGER_H
