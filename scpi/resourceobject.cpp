#include "resourceobject.h"
#include "resource/resource.h"
#include <QString>

namespace SCPI
{
  ResourceObject::ResourceObject(Application::Resource *resource) :
    cSCPIObject(),
    res(resource)
  {
  }

  bool ResourceObject::executeSCPI(const QString &sInput, QString &sOutput)
  {
    bool retVal=false;

    if(sInput.contains('?')) ///< Queries the resource
    {
      sOutput=QString("%1;%2;%3;%4;").arg(res->getAmount()).arg(res->getFreeAmount()).arg(res->getDescription()).arg(res->getPort());
      retVal=true;
    }
    return retVal;
  }
}
