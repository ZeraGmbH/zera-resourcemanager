#ifndef RESOURCEOBJECT_H
#define RESOURCEOBJECT_H
#include "scpiobject.h"

#include "resource/resource.h"

namespace SCPI
{
  class ResourceObject : public cSCPIObject
  {
  public:
    ResourceObject(Application::Resource* resource);
    bool executeSCPI(const QString& sInput, QString& sOutput);

  private:
    const Application::Resource* res;
  };
}
#endif // RESOURCEOBJECT_H
