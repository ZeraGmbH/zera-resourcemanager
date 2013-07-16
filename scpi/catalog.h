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

    /**
     * @brief executeSCPI Catalog implementation of the SCPI call
     * @param sInput
     * @param sOutput
     * @return
     */
    bool executeSCPI(const QString& sInput, QString& sOutput);

    /**
      @brief returns catalogType
      */
    const QString &getCatalogType();

    /**
     * @brief getRefCount
     * @return reference counter
     */
    quint16 getRefCount();
    /**
      @brief sets catalogType
      */
    void setCatalogType(const QString &type);
    /**
      @brief increments the reference count
      */
    void upRef();
    /**
      @brief decrements the reference count
      */
    void unRef();

  private:
    /**
      @brief the Type which the catalog represents
      */
    QString catalogType;
    /**
      @brief reference counter
      */
    quint16 refCount;
  };
}

#endif // CATALOG_H
