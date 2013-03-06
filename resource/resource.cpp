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
        occupiers.insert(occupier->getSocket(),0);
        retVal = true;
      }
    }
    return retVal;
  }

  bool Resource::freeResource(Server::Client *occupier, quint32 amount)
  {
    bool retVal = false;
    quint32 tmpVal;
    if(amount>0 && resourceAmount>0)
    {
      tmpVal=occupiers.value(occupier->getSocket(),0); // this will also detect if the occupier isn't currently holding any resource amount
      if(amount<tmpVal) //only substract the freed amount
      {
        occupiers.insert(occupier->getSocket(), tmpVal-amount);
        freeAmount=freeAmount+amount;
        retVal=true;
      }
      if(amount==tmpVal) //remove the occupier
      {
        occupiers.remove(occupier->getSocket());
        freeAmount=freeAmount+amount;
        retVal=true;
      }
    }
    else if(amount==0 && resourceAmount==0) //remove the occupier
    {
      if(occupiers.remove(occupier->getSocket())>0)
      {
        retVal=true;
      }
    }
    return retVal;
  }
}

