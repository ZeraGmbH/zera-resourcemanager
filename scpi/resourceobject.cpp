#include "resourceobject.h"
#include "resource/resource.h"
#include <QString>

namespace SCPI
{
  ResourceObject::ResourceObject(Application::Resource *resource) :
    cSCPIObject(),
    m_res(resource)
  {
  }

  bool ResourceObject::executeSCPI(const QString &sInput, QString &sOutput)
  {
    bool retVal=false;

    if(sInput.contains('?')) ///< Queries the resource
    {
      sOutput=QString("%1;%2;%3;%4;").arg(m_res->getAmount()).arg(m_res->getFreeAmount()).arg(m_res->getDescription()).arg(m_res->getPort());
      retVal=true;
    }
    return retVal;
  }
}
