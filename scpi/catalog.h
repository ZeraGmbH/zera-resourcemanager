#ifndef CATALOG_H
#define CATALOG_H

#include <scpiobject.h>
#include <QString>

namespace SCPI
{
  /**
   * @brief Provides functions to list resources of a given type
   */
  class Catalog : public cSCPIObject
  {
  public:
    Catalog();

    /**
     * @brief executeSCPI Catalog implementation of the SCPI call
     * @param sInput Query address string
     * @param sOutput List of query result nodes
     * @return true for success
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
     * @brief setCatalogType Changes the type of the SCPI::Catalog
     * @param type The new type
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
    QString m_catalogType;
    /**
      @brief reference counter
      */
    quint16 m_refCount;
  };
}

#endif // CATALOG_H
