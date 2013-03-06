#include "resource/resource.h"
#include "resourcemanager.h"


namespace Application
{
  Resource::Resource(quint32 amount, const QString& description, const QString& name, int provider, const QString& type ) :
    resourceAmount(amount), resourceDescription(description), resourceName(name), resourceProvider(provider), resourceType(type)
  {
    freeAmount=amount;
  }

  const QString &Resource::getName()
  {
    return resourceName;
  }

  int Resource::getProvider()
  {
    return resourceProvider;
  }

  const QString &Resource::getDescription()
  {
    return resourceDescription;
  }

  quint32 Resource::getAmount()
  {
    return resourceAmount;
  }

  const QString &Resource::getType()
  {
    return resourceType;
  }

  quint32 Resource::getFreeAmount()
  {
    return freeAmount;
  }

  QList<int> Resource::getOccupiers()
  {
    return occupiers.keys();
  }

  bool Resource::occupyResource(Server::Client *occupier, quint32 amount)
  {
    bool retVal = false;
    if(amount>0 && resourceAmount >0)
    {
      if(amount<=resourceAmount)
      {
        occupiers.insert(occupier->getSocket(),amount);
        freeAmount=freeAmount-amount;
        retVal = true;
      }
    }
    else if(amount==0 && resourceAmount==0)
    {
      if(occupiers.isEmpty())
      {
        occupiers.insert(occupier->getSocket(),1); //1 means the whole resource here
        retVal = true;
      }
    }
    return retVal;
  }

  bool Resource::freeResource(Server::Client *occupier)
  {
    bool retVal = false;
    quint32 tmpVal;
    tmpVal=occupiers.value(occupier->getSocket(),0); // this will also detect if the occupier isn't currently holding any resource amount
    if(tmpVal>0) //remove the occupier
    {
      occupiers.remove(occupier->getSocket());
      freeAmount=freeAmount+tmpVal;
      retVal=true;
    }
    return retVal;
  }
}
