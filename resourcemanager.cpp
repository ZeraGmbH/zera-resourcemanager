#include "resourcemanager.h"

ResourceManager::ResourceManager(QObject* parent) :
  QObject(parent)
{
}

ResourceManager::~ResourceManager()
{
  foreach(Application::Resource* r, resourceList)
  {
    r->deleteLater();
  }
}

ResourceManager  *ResourceManager::singletonInstance=0;

ResourceManager *ResourceManager::getInstance()
{
  if(singletonInstance==0)
  {
    singletonInstance=new ResourceManager;
  }
  return singletonInstance;
}

const QString ResourceManager::listResources(const QString &Type)
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

Application::Resource *ResourceManager::createResource(quint32 amount, const QString &description, const QString &name, int provider, const QString &type)
{
  Application::Resource* tmpRes=new Application::Resource(amount, description, name, provider, type);
  newResource(tmpRes);
  return tmpRes;
}

Application::Resource *ResourceManager::getResource(const QString &name, const QString &type)
{
  Application::Resource *retVal=0;
  quint32 sanityCounter=0;
  foreach (Application::Resource *res, resourceList)
  {
    if(res->getName()==name&&res->getType()==type)
    {
      sanityCounter++;
      retVal=res;
    }
  }
  if (sanityCounter>1)
  {
    //error
  }
  return retVal;
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

void ResourceManager::freeResource(Application::Resource* resource, Server::Client* client)
{
  resource->freeResource(client);
}
