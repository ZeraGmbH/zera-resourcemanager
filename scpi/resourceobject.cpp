#include "resourceobject.h"
namespace SCPI
{
  ResourceObject::ResourceObject(Application::Resource *resource) : cSCPIObject(), res(resource)
  {
  }

  bool ResourceObject::executeSCPI(const QString &sInput, QString &sOutput)
  {
    /// @todo set / free / query
    return false;
  }
}
