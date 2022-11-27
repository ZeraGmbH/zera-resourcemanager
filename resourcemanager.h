#ifndef H2012_RESOURCEMANAGER_H
#define H2012_RESOURCEMANAGER_H

#include <QObject>
#include <QSet>
#include "resman_resourceidentity.h"

/**
  @brief Represents the managing instance in this application that holds SCPI::ResourceObjects and Application::ResourceLock
  */
class ResourceManager : public QObject
{
    Q_OBJECT

public:
    explicit ResourceManager(QObject *t_parent = 0);

   /**
     * @brief keeps track of the new Application::ResourceIdentity instances
     * @param t_resourceIdentity
     */
    void addResourceIdentity(Application::ResourceIdentity *t_resourceIdentity);
    /**
      * @brief clean up the m_resourceIdentitySet from removed Application::ResourceIdentity instances
      * @param t_resourceIdentity
      */
    void removeResourceIdentity(Application::ResourceIdentity *t_resourceIdentity);

    /**
      * @brief collects a list of Application::ResourceIdentity instances the ResourceServer::IClientMultiton has occupied
      * @param t_clientMultiton
      * @return
      */
    QList<Application::ResourceIdentity *> getOccupationsByClient(ResourceServer::IClientMultiton *t_clientMultiton);
    /**
      * @brief returns a copy of the intern m_resourceIdentitySet
      * @return
      */
    QSet<Application::ResourceIdentity *> getResourceIdentitySet() const;

    template <typename T>
    /**
      * @brief getResourceIdentitiesOf
      * @param t_filter
      * @return a list with all matches is returned
      * @note comparing with Resource *, cSCPIObject * or cSCPICommand will only return one match as they only exist in 1:1 association with the resource identities
      */
    QList<Application::ResourceIdentity *> getResourceIdentitiesOf(T t_filter) const
    {
        QList<Application::ResourceIdentity *> retVal;
        QSetIterator<Application::ResourceIdentity *> setIterator(m_resourceIdentitySet);
        while(setIterator.hasNext())
        {
            Application::ResourceIdentity *riToCompare = setIterator.next();
            if(riToCompare->isAffiliatedWith<T>(t_filter))
            {
                retVal.append(riToCompare);
            }
        }

        return retVal;
    }

    template <typename T>
    /**
      * @brief getResourceIdentityOf
      * @param t_filter
      * @return the first match is returned
      * @warning comparing with Catalog * and the providing ResourceServer::ClientMultiton * can have multiple matches
      */
    Application::ResourceIdentity *getResourceIdentityOf(T t_filter) const
    {
        Application::ResourceIdentity *retVal = nullptr;
        QSetIterator<Application::ResourceIdentity *> setIterator(m_resourceIdentitySet);
        while(setIterator.hasNext())
        {
            Application::ResourceIdentity *riToCompare = setIterator.next();
            if(riToCompare->isAffiliatedWith<T>(t_filter))
            {
                retVal = riToCompare;
                break;
            }
        }

        return retVal;
    }


private:
    /**
      * @brief the ResourceManager owns the Application::ResourceIdentity entries
      */
    QSet<Application::ResourceIdentity *> m_resourceIdentitySet;
    /**
        @note Instances of this class should only get accessed through the getInstance method.
    */
    Q_DISABLE_COPY(ResourceManager)
};

#endif // H2012_RESOURCEMANAGER_H
