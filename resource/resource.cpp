#include "resource/resource.h"
#include "resourcemanager.h"

#include "server/clientmultiton.h"
#include <QDebug>
namespace Application
{
  Resource::Resource(quint32 amount, const QString& description, const QString& name, Server::ClientMultiton *provider, const QString& type , quint32 port, const QByteArray providerId) :
    m_resourceAmount(amount),
    m_resourceDescription(description),
    m_resourceName(name),
    m_resourcePort(port),
    m_resourceProvider(provider),
    m_resourceProviderId(providerId),
    m_resourceType(type)
  {
    //qDebug() << "providerId:" << providerId;
    m_resourceObject=0;
    m_freeAmount=amount;
  }

  const QString &Resource::getName()
  {
    return m_resourceName;
  }

  Server::ClientMultiton *Resource::getProvider()
  {
    return m_resourceProvider;
  }

  const QString &Resource::getDescription()
  {
    return m_resourceDescription;
  }

  quint32 Resource::getAmount()
  {
    return m_resourceAmount;
  }

  SCPI::ResourceObject *Resource::getResourceObject()
  {
    return m_resourceObject;
  }

  const QString &Resource::getType()
  {
    return m_resourceType;
  }

  quint32 Resource::getFreeAmount()
  {
    return m_freeAmount;
  }

  QList<Server::ClientMultiton *> Resource::getOccupiers()
  {
    return m_occupiers.keys();
  }

  quint32 Resource::getPort() const
  {
    return m_resourcePort;
  }

  const QByteArray &Resource::getProviderId() const
  {
    return m_resourceProviderId;
  }

  bool Resource::occupyResource(Server::ClientMultiton *occupier, quint32 amount)
  {
    bool retVal = false;
    if(amount>0 && m_resourceAmount >0)
    {
      if(amount<=m_resourceAmount)
      {
        occupier->addOccupation(this);
        m_occupiers.insert(occupier,amount);
        m_freeAmount=m_freeAmount-amount;
        retVal = true;
      }
    }
    else if(amount==0 && m_resourceAmount==0)
    {
      if(m_occupiers.isEmpty())
      {
        occupier->addOccupation(this);
        m_occupiers.insert(occupier,1); //1 means the whole resource here
        retVal = true;
      }
    }
    return retVal;
  }

  bool Resource::freeResource(Server::ClientMultiton *occupier)
  {
    bool retVal = false;
    quint32 tmpVal;
    tmpVal=m_occupiers.value(occupier,0); // this will also detect if the occupier isn't currently holding any resource amount
    if(tmpVal>0) //remove the occupier
    {
      occupier->removeOccupation(this);
      m_occupiers.remove(occupier);
      m_freeAmount=m_freeAmount+tmpVal;
      retVal=true;
    }
    return retVal;
  }

  void Resource::setObject(SCPI::ResourceObject *obj)
  {
    if(!m_resourceObject)
    {
      m_resourceObject=obj;
    }
  }
}
