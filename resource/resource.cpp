#include "resource/resource.h"
#include "resourcemanager.h"


namespace Application
{
  Resource::Resource(quint32 amount, const QString& description, const QString& name, Server::Client * provider, const QString& type ) :
    resourceAmount(amount), resourceDescription(description), resourceName(name), resourceProvider(provider), resourceType(type)
  {
    freeAmount=amount;
  }

  const QString &Resource::getName()
  {
    return resourceName;
  }

  Server::Client * Resource::getProvider()
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

  QList<Server::Client *> Resource::getOccupiers()
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
        occupiers.insert(occupier,amount);
        freeAmount=freeAmount-amount;
        retVal = true;
      }
    }
    else if(amount==0 && resourceAmount==0)
    {
      if(occupiers.isEmpty())
      {
        occupiers.insert(occupier,1); //1 means the whole resource here
        retVal = true;
      }
    }
    return retVal;
  }

  bool Resource::freeResource(Server::Client *occupier)
  {
    bool retVal = false;
    quint32 tmpVal;
    tmpVal=occupiers.value(occupier,0); // this will also detect if the occupier isn't currently holding any resource amount
    if(tmpVal>0) //remove the occupier
    {
      occupiers.remove(occupier);
      freeAmount=freeAmount+tmpVal;
      retVal=true;
    }
    return retVal;
  }
}
