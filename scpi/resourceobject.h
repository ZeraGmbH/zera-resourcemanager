#ifndef RESOURCEOBJECT_H
#define RESOURCEOBJECT_H

#include <scpiobject.h>

namespace Application
{
  class Resource;
}

namespace SCPI
{
  /**
    @brief Represents the resource in the SCPI world
    */
  class ResourceObject : public cSCPIObject
  {
  public:
    /**
      @brief this constructor will set the const pointer to the Application::Resource
      */
    ResourceObject(Application::Resource* resource);
    bool executeSCPI(const QString& sInput, QString& sOutput);

  private:
    /**
      @brief A link to the corresponding Application::Resource
      */
    Application::Resource* m_res;
  };
}
#endif // RESOURCEOBJECT_H
