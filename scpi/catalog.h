#ifndef CATALOG_H
#define CATALOG_H

#include <QString>
#include "scpiobject.h"


namespace SCPI
{
  /**
    @brief Allows to list resources of a given type
    */
  class Catalog : public cSCPIObject
  {
  public:
    Catalog();

    bool executeSCPI(const QString& sInput, QString& sOutput);

    /**
      @b returns catalogType
      */
    const QString &getCatalogType();
    /**
      @b sets catalogType
      */
    void setCatalogType(const QString &type);
    /**
      @b increments the reference count
      */
    void upRef();
    /**
      @b decrements the reference count
      */
    void unRef();

  private:
    /**
      @b the Type which the catalog represents
      */
    QString catalogType;
    /**
      @b reference counter
      */
    int refCount;
  };
}

#endif // CATALOG_H
