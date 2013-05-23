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

    if(sInput.contains('?')) ///< @b Queries the resource
    {
      sOutput=QString("%1;%2;%3;").arg(res->getAmount()).arg(res->getFreeAmount()).arg(res->getDescription());
      retVal=true;
    }
    return retVal;
  }
}
