#include "catalog.h"

namespace SCPI
{
  Catalog::Catalog() : cSCPIObject()
  {
    refCount=0;
  }

  bool Catalog::executeSCPI(const QString &sInput, QString &sOutput)
  {
    /// @todo !!
    //sOutput=ResourceManager::getInstance()->listResources(sInput);
    return true;
  }

  const QString &Catalog::getCatalogType()
  {
    return catalogType;
  }

  quint16 Catalog::getRefCount()
  {
    return refCount;
  }

  void Catalog::setCatalogType(const QString& type)
  {
    catalogType=type;
  }

  void Catalog::upRef()
  {
    refCount++;
  }

  void Catalog::unRef()
  {
    if(refCount>0)
    {
      refCount--;
    }
    else
    {
      // error
    }
  }

}
