#include "resourcemanager.h"

ResourceManager::ResourceManager(QObject* parent) :
  QObject(parent)
{
}

ResourceManager* ResourceManager::singletonInstance=0;

ResourceManager* ResourceManager::getInstance()
{
  if(singletonInstance==0)
  {
    singletonInstance=new ResourceManager;
  }
  return singletonInstance;
}

const QString &ResourceManager::listResources(const QString &Type)
{
  Application::Resource* tmpRO;
  QString retVal;
  foreach(tmpRO, resourceList)
  {
    if(tmpRO->getType()==Type)
      retVal+=tmpRO->getName()+";";
  }
  return retVal;
}

Application::Resource* ResourceManager::createResource(quint32 amount, const QString &description, const QString &name, int provider, const QString &type)
{
  Application::Resource* tmpRes=new Application::Resource(amount, description, name, provider, type);
  newResource(tmpRes);
  return tmpRes;
}

void ResourceManager::newResource(Application::Resource* resource)
{
  resourceList.append(resource);
}

void ResourceManager::deleteResource(Application::Resource* resource)
{
  resourceList.removeAll(resource);

}

void ResourceManager::occupyResource(Application::Resource* resource, Server::Client* client, quint32 amount)
{
  if(resource->occupyResource(client,amount))
  {
    Server::ServerInterface::getInstance()->lockGranted(resource,client);
  }
  else
  {
    Server::ServerInterface::getInstance()->lockFailed(resource,client);
  }
}

void ResourceManager::freeResource(Application::Resource* resource, Server::Client* client, quint32 amount)
{
  resource->freeResource(client, amount);
}
