#ifndef APPLICATION_RESOURCEIDENTITY_H
#define APPLICATION_RESOURCEIDENTITY_H

#include <QtGlobal>
#include <scpicommand.h>

class cSCPIObject;

namespace SCPI
{
class Catalog;
class SCPIInterface;
}

namespace ResourceServer
{
class IClientMultiton;
}

namespace Application
{
class Resource;

class ResourceIdentity
{
public:
    /**
     * @brief ResourceIdentity
     * @param t_resource ResourceIdentity takes ownership of the Application::Resource
     * @param t_provider ResourceIdentity does not take ownership of the ResourceServer::IClientMultiton
     * @param t_catalog ResourceIdentity takes ownership of the SCPI::Catalog
     * @param t_scpiObject ResourceIdentity takes ownership of the cSCPIObject
     * @param t_scpiCommand
     */
    ResourceIdentity(Resource *t_resource, ResourceServer::IClientMultiton *t_provider, SCPI::Catalog *t_catalog, cSCPIObject *t_scpiObject, cSCPICommand t_scpiCommand);
    ~ResourceIdentity();
    /**
     * @brief sets the interface
     * @param t_interface
     * @badcode must be called before the constructor
     */
    static void setSCPIInterface(SCPI::SCPIInterface *t_interface);

    /**
     * @brief occupies the resource for client t_clientMultiton
     * @param t_clientMultiton
     * @param t_amount the amount to occupy
     * @return true for success
     */
    bool occupyResource(ResourceServer::IClientMultiton *t_clientMultiton, quint32 t_amount);
    /**
     * @brief releases the occupation of the resource made previously from client t_clientMultiton
     * @param t_clientMultiton
     * @return true for success
     */
    bool releaseResource(ResourceServer::IClientMultiton *t_clientMultiton);

    const Resource *getResource() const;
    const ResourceServer::IClientMultiton *getProvider() const;
    SCPI::Catalog *getCatalog() const;
    const cSCPIObject *getSCPIObject() const;
    cSCPICommand getSCPICommand() const;

    /**
     * @brief getOccupationAmountOf
     * @param t_clientMultiton
     * @return returns the occupation amount of t_clientMultiton
     */
    quint32 getOccupationAmountOf(const ResourceServer::IClientMultiton *t_clientMultiton) const;
    /**
     * @brief getFreeAmount
     * @return returns the total amount - every occupied amount
     */
    quint32 getFreeAmount() const;

    template <typename T>
    /**
     * @brief template based comparision of properties in this ResourceIdentity
     * @param t_filter determines which overload of isAffiliatedWithImpl is called
     * @return true if t_filter matches a member of this ResourceIdentity
     */
    bool isAffiliatedWith(T t_filter) const
    {
        return isAffiliatedWithImpl(t_filter);
    }

private:
    /**
     * @brief Reference houskeeping for Catalog instances
     */
    void upRefCatalog();
    /**
     * @brief Reference houskeeping for Catalog instances
     * @note if the Catalog reaches refcount == 0, then it will be deleted as it is not referenced anymore
     * @note also removes the cSCPICommand for the catalog if it will be deleted
     */
    void unRefCatalog();

    bool isAffiliatedWithImpl(const Resource *t_filter) const;
    bool isAffiliatedWithImpl(const ResourceServer::IClientMultiton *t_filter) const;
    bool isAffiliatedWithImpl(const SCPI::Catalog *t_filter) const;
    bool isAffiliatedWithImpl(const cSCPIObject *t_filter) const;
    bool isAffiliatedWithImpl(const cSCPICommand t_filter) const;

    /**
     * @brief holds the actual resource data
     */
    const Resource *m_resource = nullptr;
    /**
     * @brief points to the creator of the resource
     */
    const ResourceServer::IClientMultiton *m_provider = nullptr;
    /**
     * @brief a shared pointer to the catalog the resource belongs to
     * @note a refcount is used to track the Catalog usage
     */
    SCPI::Catalog *m_catalog = nullptr;
    /**
     * @brief the cSCPIObject in the scpi node tree that represents m_resource
     */
    const cSCPIObject *m_scpiObject = nullptr;
    /**
     * @brief the cSCPICommand of the scpi node
     */
    const cSCPICommand m_scpiCommand;
    /**
     * @brief tracks occupations of m_resource
     */
    QHash <const ResourceServer::IClientMultiton *, quint32> m_occupiers;
    /**
     * @brief common scpi interface
     */
    static SCPI::SCPIInterface *s_scpiInterface;

    Q_DISABLE_COPY(ResourceIdentity)
};
} // namespace Application

#endif // APPLICATION_RESOURCEIDENTITY_H
