#include "catalog.h"
#include "resourcemanager.h"
#include <QStringList>

namespace SCPI
{
  Catalog::Catalog() : cSCPIObject()
  {
    m_refCount=0;
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
    return m_catalogType;
  }

  quint16 Catalog::getRefCount()
  {
    return m_refCount;
  }

  void Catalog::setCatalogType(const QString& type)
  {
    m_catalogType=type;
  }

  void Catalog::upRef()
  {
    m_refCount++;
  }

  void Catalog::unRef()
  {
    if(m_refCount>0)
    {
      m_refCount--;
    }
    else
    {
      // error
    }
  }

}
