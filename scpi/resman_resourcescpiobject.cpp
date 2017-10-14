#include "resman_resourcescpiobject.h"
#include "resource/resman_resource.h"
#include "resource/resman_resourceidentity.h"
#include <QString>

namespace SCPI
{
  ResourceSCPIObject::ResourceSCPIObject() :
    cSCPIObject()
  {
  }

  bool ResourceSCPIObject::executeSCPI(const QString &t_input, QString &t_output)
  {
    bool retVal=false;

    if(t_input.contains('?')) ///< Queries the resource
    {
      const Application::Resource *tmpResource = m_resourceIdentity->getResource();
      t_output=QString("%1;%2;%3;%4;").arg(tmpResource->getAmount()).arg(m_resourceIdentity->getFreeAmount()).arg(tmpResource->getDescription()).arg(tmpResource->getPort());
      retVal=true;
    }
    return retVal;
  }

  void ResourceSCPIObject::setResourceIdentity(Application::ResourceIdentity *t_resourceIdentity)
  {
    //there is no point to set the resource identity if it has been set previously
    Q_ASSERT(m_resourceIdentity == nullptr);
    Q_ASSERT(t_resourceIdentity != nullptr);
    m_resourceIdentity = t_resourceIdentity;
  }
}
