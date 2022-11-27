#include "resman_catalog.h"
#include "resourcemanager.h"
#include "resman_resource.h"
#include "resman_resourceidentity.h"
#include <QStringList>

namespace SCPI
{
Catalog::Catalog(ResourceManager *t_resMan, const QString &scpiName, quint8 scpiType) :
    cSCPIObject(scpiName, scpiType),
    m_resMan(t_resMan)
{
}

bool Catalog::executeSCPI(const QString &t_input, QString &t_output)
{
    Q_UNUSED(t_input);
    QStringList resourceNames;

    const QList<Application::ResourceIdentity *> tmpResourcesOfCatalog = m_resMan->getResourceIdentitiesOf<Catalog *>(this);
    for(const Application::ResourceIdentity *tmpRes : qAsConst(tmpResourcesOfCatalog))
    {
        resourceNames.append(tmpRes->getResource()->getName());
    }
    t_output=resourceNames.join(';');

    return !t_output.isEmpty();
}

const QString &Catalog::getCatalogType() const
{
    return m_catalogType;
}

quint16 Catalog::getRefCount() const
{
    return m_refCount;
}

void Catalog::setCatalogType(const QString& t_type)
{
    m_catalogType=t_type;
}

void Catalog::upRef()
{
    m_refCount++;
}

void Catalog::unRef()
{
    Q_ASSERT(m_refCount>0);
    m_refCount--;
}
}
