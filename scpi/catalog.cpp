#include "catalog.h"
#include "resourcemanager.h"
#include <QStringList>

namespace SCPI
{
  Catalog::Catalog() : cSCPIObject()
  {
    refCount=0;
  }

  bool Catalog::executeSCPI(const QString &sInput, QString &sOutput)
  {
    QString resType;
    QStringList separator;
    separator=sInput.split(":");
    resType=separator.at(separator.count()-2);//get the element before the last
    sOutput=ResourceManager::getInstance()->listResources(resType);

    if(sOutput.isEmpty())
      return false;
    else
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
