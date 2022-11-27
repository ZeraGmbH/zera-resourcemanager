#include "resourcemanager.h"


ResourceManager::ResourceManager(QObject *t_parent) :
    QObject(t_parent)
{
}

void ResourceManager::addResourceIdentity(Application::ResourceIdentity *t_resourceIdentity)
{
    Q_ASSERT(t_resourceIdentity != nullptr);
    m_resourceIdentitySet.insert(t_resourceIdentity);
}

void ResourceManager::removeResourceIdentity(Application::ResourceIdentity *t_resourceIdentity)
{
    Q_ASSERT(t_resourceIdentity != nullptr);
    if(m_resourceIdentitySet.contains(t_resourceIdentity))
    {
        m_resourceIdentitySet.remove(t_resourceIdentity);
        delete t_resourceIdentity;
    }
}

QList<Application::ResourceIdentity *> ResourceManager::getOccupationsByClient(ResourceServer::IClientMultiton *t_clientMultiton)
{
    QList<Application::ResourceIdentity *> retVal;
    QSetIterator<Application::ResourceIdentity *> setIterator(m_resourceIdentitySet);
    while(setIterator.hasNext())
    {
        Application::ResourceIdentity *riToCompare = setIterator.next();
        if(riToCompare->getOccupationAmountOf(t_clientMultiton) > 0)
        {
            retVal.append(riToCompare);
        }
    }

    return retVal;
}

QSet<Application::ResourceIdentity *> ResourceManager::getResourceIdentitySet() const
{
    return m_resourceIdentitySet;
}
