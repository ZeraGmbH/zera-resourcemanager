#ifndef CATALOG_H
#define CATALOG_H

#include <scpiobject.h>
#include <QString>

class ResourceManager;

namespace SCPI
{
/**
  * @brief Provides functions to list resources of a given type
  */
class Catalog : public cSCPIObject
{
public:
    Catalog(ResourceManager *t_resMan, const QString &scpiName, quint8 scpiType);

    /**
     * @brief executeSCPI Catalog implementation of the SCPI call
     * @param t_input Query address string
     * @param t_output List of query result nodes
     * @return true for success
     */
    bool executeSCPI(const QString& t_input, QString& t_output) override;

    /**
      @brief returns catalogType
      */
    const QString &getCatalogType() const;

    /**
     * @brief getRefCount
     * @return reference counter
     */
    quint16 getRefCount() const;
    /**
     * @brief setCatalogType Changes the type of the SCPI::Catalog
     * @param t_type The new type
     */
    void setCatalogType(const QString &t_type);
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
    quint16 m_refCount = 0;

    const ResourceManager *m_resMan = nullptr;
    Q_DISABLE_COPY(Catalog)
};
}

#endif // CATALOG_H
