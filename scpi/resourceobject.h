#ifndef RESOURCEOBJECT_H
#define RESOURCEOBJECT_H
#include "scpiobject.h"

#include "resource/resource.h"

namespace SCPI
{
  /**
    @brief Represents the resource in the SCPI world
    */
  class ResourceObject : public cSCPIObject
  {
  public:
    /**
      @b this constructor will set the const pointer to the Application::Resource
      */
    ResourceObject(Application::Resource* resource);
    bool executeSCPI(const QString& sInput, QString& sOutput);

  private:
    /**
      @b A link to the corresponding Application::Resource
      */
    const Application::Resource* res;
  };
}
#endif // RESOURCEOBJECT_H
