#include "resourcemanager.h"
#include "scpistring.h"
#include "server/client.h"
#include "server/serverinterface.h"
#include "resource/resource.h"

ResourceManager::ResourceManager(QObject* parent) :
  QObject(parent)
{
}

ResourceManager::~ResourceManager()
{
  foreach(Application::Resource* r, m_resourceList)
  {
    r->deleteLater();
  }
}

ResourceManager  *ResourceManager::m_singletonInstance=0;

ResourceManager *ResourceManager::getInstance()
{
  if(m_singletonInstance==0)
  {
    m_singletonInstance=new ResourceManager;
  }
  return m_singletonInstance;
}

const QString ResourceManager::listResources(const QString &Type)
{
  cSCPIString a, b;
  QString retVal;
  a=Type;
  foreach(Application::Resource* tmpRO, m_resourceList)
  {
    b=tmpRO->getType();
    if(a==b)
      retVal+=tmpRO->getName()+";";
  }
  return retVal;
}

Application::Resource *ResourceManager::createResource(quint32 amount, const QString &description, const QString &name, Server::Client * provider, const QString &type, quint32 port, const QByteArray &providerId)
{
  Application::Resource* tmpRes=new Application::Resource(amount, description, name, provider, type, port, providerId);
  onResourceAdded(tmpRes);
  return tmpRes;
}

Application::Resource *ResourceManager::getResourceByObject(SCPI::ResourceObject* obj)
{
  Application::Resource *retVal=0;
  quint32 sanityCounter=0;
  foreach (Application::Resource *res, m_resourceList)
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
  foreach (Application::Resource *res, m_resourceList)
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

void ResourceManager::onResourceAdded(Application::Resource* resource)
{
  m_resourceList.append(resource);
}

void ResourceManager::onResourceRemoved(Application::Resource* resource)
{
  m_resourceList.removeAll(resource);
}
