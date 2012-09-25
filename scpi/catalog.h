#ifndef CATALOG_H
#define CATALOG_H

#include <QString>
#include "scpiobject.h"


namespace SCPI
{
  /**
    @b Allows to list resources of a given type
    */
  class Catalog : public cSCPIObject
  {
  public:
    Catalog();

    bool executeSCPI(const QString& sInput, QString& sOutput);

    QString getCatalogType();
    void setCatalogType(const QString &type);
    void upRef();
    void unRef();

  private:
    static Catalog* singletonInstance;
    QString catalogType;
    int refCount;
  };
}

#endif // CATALOG_H
