#include "resource/resourceobject.h"

namespace Resource
{
  ResourceObject::ResourceObject(QString name, Server::Client *provider, QString description, quint32 amount) :
    resourceName(name), resourceProvider(provider), resourceDescription(description), resourceAmount(amount)
  {
    freeAmount=amount;
  }

  const QString ResourceObject::getName()
  {
    return resourceName;
  }

  Server::Client *ResourceObject::getProvider()
  {
    return resourceProvider;
  }

  const QString ResourceObject::getDescription()
  {
    return resourceDescription;
  }

  quint32 ResourceObject::getAmount()
  {
    return resourceAmount;
  }

  quint32 ResourceObject::getFreeAmount()
  {
    return freeAmount;
  }

  QList<Server::Client *> ResourceObject::getOccupiers()
  {
    return occupiers.keys();
  }

  bool ResourceObject::occupyResource(Server::Client *occupier, quint32 amount)
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
        occupiers.insert(occupier,0);
        retVal = true;
      }
    }
    return retVal;
  }

  bool ResourceObject::freeResource(Server::Client *occupier, quint32 amount)
  {
    bool retVal = false;
    quint32 tmpVal;
    if(amount>0 && resourceAmount>0)
    {
      tmpVal=occupiers.value(occupier,0); // this will also detect if the occupier isn't currently holding any resource amount
      if(amount<tmpVal) //only substract the freed amount
      {
        occupiers.insert(occupier, tmpVal-amount);
        freeAmount=freeAmount+amount;
        retVal=true;
      }
      if(amount==tmpVal) //remove the occupier
      {
        occupiers.remove(occupier);
        freeAmount=freeAmount+amount;
        retVal=true;
      }
    }
    else if(amount==0 && resourceAmount==0) //remove the occupier
    {
      if(occupiers.remove(occupier)>0)
      {
        retVal=true;
      }
    }
    return retVal;
  }
}

