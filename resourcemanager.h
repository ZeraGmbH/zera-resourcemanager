#ifndef H2012_RESOURCEMANAGER_H
#define H2012_RESOURCEMANAGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QTextStream>
#include "server/client.h"
#include "server/serverinterface.h"
#include "resource/resource.h"

/**
  @brief Represents the managing instance in this application that holds SCPI::ResourceObjects and Application::ResourceLock
  */
class ResourceManager : public QObject
{
  Q_OBJECT
protected:
  /**
    @b The class is a Singleton so the constructor is protected [P.157+ Design patterns Gang of Four]
    */
  ResourceManager(QObject* parent = 0);
  ~ResourceManager();

public:
  /**
    @b See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* getInstance();

  /**
    @b Lists all resources of a given type as QString
    @returns QString with resources separated by a semicolon
    */
  const QString listResources(const QString &Type);

  /**
    @b Creates a Application::Resource and appends it to the resourceList
    */
  Application::Resource *createResource(quint32 amount, const QString &description, const QString &name, Server::Client * provider, const QString &type);

  /**
    @b retrieves the resource with the given name and type
    */
  Application::Resource *getResourceByName(const QString &name, const QString &type);

  /**
   * @brief Find a Resource with its ResourceObject
   * @param obj the objec that represents the resource
   * @return the resource
   */
  Application::Resource *getResourceByObject(SCPI::ResourceObject* obj);

public slots:
  /**
    @b Will be triggered when the a Server::Client provides a new Resource:ResourceObject
    @param [in] *resource the SCPI::ResourceObject that will be added in ResourceManager
    */
  void newResource(Application::Resource* resource);

  /**
    @b Will be triggered when the a Server::Client wants to delete a Resource:ResourceObject
    @param [in] *resource the SCPI::ResourceObject that will be  deleted in ResourceManager
    */
  void deleteResource(Application::Resource* resource);

private:
  /**
    @b See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* singletonInstance;

  /**
    @b Holds the resources
    */
  QList<Application::Resource*> resourceList;
  /**
    @note Instances of this class should only get accessed through the getInstance method.
    */
  Q_DISABLE_COPY(ResourceManager)
};

#endif // H2012_RESOURCEMANAGER_H
