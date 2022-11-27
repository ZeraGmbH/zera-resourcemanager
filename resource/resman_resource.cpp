#include "resman_resource.h"

#include "resman_clientmultiton.h"
#include <QDebug>
namespace Application
{
Resource::Resource(quint32 t_amount, const QString& t_description, const QString& t_name, const QString& t_type, quint16 t_port) :
    m_resourceAmount(t_amount),
    m_resourceDescription(t_description),
    m_resourceName(t_name),
    m_resourcePort(t_port),
    m_resourceType(t_type)
{
}

const QString &Resource::getName() const
{
    return m_resourceName;
}

const QString &Resource::getDescription() const
{
    return m_resourceDescription;
}

quint32 Resource::getAmount() const
{
    return m_resourceAmount;
}

const QString &Resource::getType() const
{
    return m_resourceType;
}

quint16 Resource::getPort() const
{
    return m_resourcePort;
}
}
