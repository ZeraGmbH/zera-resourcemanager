#ifndef H2012_RESOURCEMANAGER_H
#define H2012_RESOURCEMANAGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QTextStream>

namespace Application
{
  class Resource;
}

namespace SCPI
{
  class ResourceObject;
}

namespace Server
{
  class Client;
}
/**
  @brief Represents the managing instance in this application that holds SCPI::ResourceObjects and Application::ResourceLock
  */
class ResourceManager : public QObject
{
  Q_OBJECT
protected:
  /**
    @brief The class is a Singleton so the constructor is protected [P.157+ Design patterns Gang of Four]
    */
  ResourceManager(QObject* parent = 0);
  ~ResourceManager();

public:
  /**
    @brief See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* getInstance();

  /**
    @brief Lists all resources of a given type as QString
    @returns QString with resources separated by a semicolon
    */
  const QString listResources(const QString &Type);

  /**
    @brief Creates a Application::Resource and appends it to the resourceList
    */
  Application::Resource *createResource(quint32 amount, const QString &description, const QString &name, Server::Client * provider, const QString &type, quint32 port=0, const QByteArray &providerId=0);

  /**
    @brief retrieves the resource with the given name and type
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
    @brief Will be triggered when the a Server::Client provides a new Resource:ResourceObject
    @param [in] *resource the SCPI::ResourceObject that will be added in ResourceManager
    */
  void onResourceAdded(Application::Resource* resource);

  /**
    @brief Will be triggered when the a Server::Client wants to delete a Resource:ResourceObject
    @param [in] *resource the SCPI::ResourceObject that will be  deleted in ResourceManager
    */
  void onResourceRemoved(Application::Resource* resource);

private:
  /**
    @brief See [P.157+ Design patterns Gang of Four]
    */
  static ResourceManager* m_singletonInstance;

  /**
    @brief Holds the resources
    */
  QList<Application::Resource*> m_resourceList;
  /**
    @note Instances of this class should only get accessed through the getInstance method.
    */
  Q_DISABLE_COPY(ResourceManager)
};

#endif // H2012_RESOURCEMANAGER_H
