#include "resourcemanager.h"
#include "scpistring.h"

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
  cSCPIString a, b;
  QString retVal;
  a=Type;
  foreach(Application::Resource* tmpRO, resourceList)
  {
    b=tmpRO->getType();
    if(a==b)
      retVal+=tmpRO->getName()+";";
  }
  return retVal;
}

Application::Resource *ResourceManager::createResource(quint32 amount, const QString &description, const QString &name, Server::Client * provider, const QString &type)
{
  Application::Resource* tmpRes=new Application::Resource(amount, description, name, provider, type);
  newResource(tmpRes);
  return tmpRes;
}

Application::Resource *ResourceManager::getResourceByObject(SCPI::ResourceObject* obj)
{
  Application::Resource *retVal=0;
  quint32 sanityCounter=0;
  foreach (Application::Resource *res, resourceList)
  {
    if(res->getResourceObject()==obj)
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

Application::Resource *ResourceManager::getResourceByName(const QString &name, const QString &type)
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
