#ifndef H2012_RESOURCEMANAGER_H
#define H2012_RESOURCEMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "server/client.h"
#include "resource/resourceobject.h"



/**
  @b Represents the managing instance in this application that holds Resource::ResourceObjects and Resource::ResourceLock
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
    @b returns the model
    */
  QStandardItemModel* getModel();

signals:
  /**
    @b Will be triggered when the a Server::Client wants to lock a Resource:ResourceObject but it is already locked
    @param [in] *resource the Resource::ResourceObject that will be locked with Resource::ResourceLock
    @param [in] *client the Server::Client that requested the operation
    */
  void lockFailed(Resource::ResourceObject* resource, Server::Client* client);

  /**
    @b Will be triggered when the a Server::Client wants to lock a Resource:ResourceObject and it is lockable
    @param [in] *resource the Resource::ResourceObject that will be locked with Resource::ResourceLock
    @param [in] *client the Server::Client that requested the operation
    */
  void lockGranted(Resource::ResourceObject* resource, Server::Client* client);

public slots:
  /**
    @b Will be triggered when the a Server::Client provides a new Resource:ResourceObject
    @param [in] *resource the Resource::ResourceObject that will be added in ResourceManager
    */
  void newResource(Resource::ResourceObject* resource);

  /**
    @b Will be triggered when the a Server::Client wants to delete a Resource:ResourceObject
    @param [in] *resource the Resource::ResourceObject that will be  deleted in ResourceManager
      */
  void deleteResource(Resource::ResourceObject* resource);

  /**
    @b Will be triggered when the a Server::Client wants to lock a Resource:ResourceObject
    @param [in] *resource the Resource::ResourceObject that will be locked with Resource::ResourceLock
    @param [in] *client the Server::Client that attempts to lock the Resource::ResourceObject
    @param amount if the resource has a quantity the amount of the resource that should be reserved can be specified
    */
  void occupyResource(Resource::ResourceObject* resource, Server::Client* client, quint32 amount=0);

  /**
    @b Will be triggered when the a Server::Client wants to unlock Resource:ResourceObject
    @param [in] *resource the Resource::ResourceObject that will be unlocked with Resource::ResourceLock
    @param [in] *client the Server::Client requesting the operation
    @param amount if the resource has a quantity the amount of the resource that should be freed can be specified
    */
  void freeResource(Resource::ResourceObject* resource, Server::Client* client, quint32 amount=0);

private:
  /**
    @b See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* singletonInstance;

  /**
    @b The list of Resource::ResourceObject instances that are managed by this class
    */
 QStandardItemModel resourceModel;

  /**
    @note Instances of this class should only get accessed through the getInstance method.
    */
  Q_DISABLE_COPY(ResourceManager)
};

#endif // H2012_RESOURCEMANAGER_H
