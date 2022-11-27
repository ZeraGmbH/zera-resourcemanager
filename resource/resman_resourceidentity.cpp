#include "resman_resourceidentity.h"

#include "resman_resource.h"
#include "resman_catalog.h"
#include "resman_scpiinterface.h"
#include "resman_clientmultiton.h"
#include <QDebug>

namespace Application
{
ResourceIdentity::ResourceIdentity(Resource *t_resource, ResourceServer::IClientMultiton *t_provider, SCPI::Catalog *t_catalog, cSCPIObject *t_scpiObject, cSCPICommand t_scpiCommand) :
    m_resource(t_resource), m_provider(t_provider), m_catalog(t_catalog), m_scpiObject(t_scpiObject) , m_scpiCommand(t_scpiCommand)
{
    Q_ASSERT(s_scpiInterface != nullptr);
    Q_ASSERT(m_resource != nullptr);
    Q_ASSERT(m_provider != nullptr);
    Q_ASSERT(m_catalog != nullptr);
    Q_ASSERT(m_scpiObject != nullptr);
    upRefCatalog();
}

ResourceIdentity::~ResourceIdentity()
{
    delete m_resource;
    //do not delete the provider here
    unRefCatalog();
    delete m_scpiObject;
    s_scpiInterface->removeSCPICommand(m_scpiCommand);
}

void ResourceIdentity::setSCPIInterface(SCPI::SCPIInterface *t_interface)
{
    Q_ASSERT(t_interface != nullptr);
    s_scpiInterface = t_interface;
}

//  bool ResourceIdentity::operator ==(const ResourceIdentity &t_other) const
//  {
//    return (t_other.m_catalog == m_catalog &&
//            t_other.m_provider == m_provider &&
//            t_other.m_resource == m_resource &&
//            t_other.m_scpiObject == m_scpiObject &&
//            t_other.m_scpiCommand.getCommand() == m_scpiCommand.getCommand());
//  }

bool ResourceIdentity::occupyResource(ResourceServer::IClientMultiton *t_clientMultiton, quint32 t_amount)
{
    Q_ASSERT(t_clientMultiton != nullptr);

    bool retVal = false;
    quint32 freeAmount = m_resource->getAmount();
    if(freeAmount == 0 && t_amount == 0) //special case for resources with amount 0 (they really should have amount 1 instead)
    {
        qWarning() << "Resources with amount 0 are deprecated, resource:" << m_resource->getName() << "caller:" << t_clientMultiton->getName();
        freeAmount = 1;
        t_amount = 1;
    }

    if(t_amount>0)
    {
        const QList<quint32> alreadyOccupied = m_occupiers.values();
        for(const quint32 occupationAmount : qAsConst(alreadyOccupied))
        {
            freeAmount -= occupationAmount;
        }

        if(freeAmount >= t_amount)
        {
            quint32 newOccupationAmount = m_occupiers.value(t_clientMultiton, 0) + t_amount;
            m_occupiers.insert(t_clientMultiton, newOccupationAmount);
            retVal = true;
        }
    }
    return retVal;
}

bool ResourceIdentity::releaseResource(ResourceServer::IClientMultiton *t_clientMultiton)
{
    Q_ASSERT(t_clientMultiton != nullptr);

    bool retVal = false;
    if(m_occupiers.contains(t_clientMultiton))
    {
        retVal = true;
        m_occupiers.remove(t_clientMultiton);
    }

    return retVal;
}

const Resource *ResourceIdentity::getResource() const
{
    return m_resource;
}

const ResourceServer::IClientMultiton *ResourceIdentity::getProvider() const
{
    return m_provider;
}

SCPI::Catalog *ResourceIdentity::getCatalog() const
{
    return m_catalog;
}

const cSCPIObject *ResourceIdentity::getSCPIObject() const
{
    return m_scpiObject;
}

cSCPICommand ResourceIdentity::getSCPICommand() const
{
    return m_scpiCommand;
}

quint32 ResourceIdentity::getOccupationAmountOf(const ResourceServer::IClientMultiton *t_clientMultiton) const
{
    return m_occupiers.value(t_clientMultiton, 0);
}

quint32 ResourceIdentity::getFreeAmount() const
{
    quint32 freeAmount = m_resource->getAmount();
    const QList<quint32> alreadyOccupied = m_occupiers.values();
    for(const quint32 occupationAmount : qAsConst(alreadyOccupied))
    {
        freeAmount -= occupationAmount;
    }
    return freeAmount;
}

void ResourceIdentity::upRefCatalog()
{
    m_catalog->upRef();
}

void ResourceIdentity::unRefCatalog()
{
    m_catalog->unRef();
    if(m_catalog->getRefCount() == 0)
    {
        s_scpiInterface->removeSCPICommand(cSCPICommand(QString("RESOURCE:%1:CATALOG").arg(m_catalog->getCatalogType())));
        delete m_catalog;
    }
}

bool ResourceIdentity::isAffiliatedWithImpl(const Resource *t_filter) const
{
    return m_resource == t_filter;
}

bool ResourceIdentity::isAffiliatedWithImpl(const ResourceServer::IClientMultiton *t_filter) const
{
    return m_provider == t_filter;
}

bool ResourceIdentity::isAffiliatedWithImpl(const SCPI::Catalog *t_filter) const
{
    return m_catalog == t_filter;
}

bool ResourceIdentity::isAffiliatedWithImpl(const cSCPIObject *t_filter) const
{
    return m_scpiObject == t_filter;
}

bool ResourceIdentity::isAffiliatedWithImpl(const cSCPICommand t_filter) const
{
    return m_scpiCommand.getCommand() == t_filter.getCommand();
}

SCPI::SCPIInterface *ResourceIdentity::s_scpiInterface = nullptr;
} // namespace Application
