#ifndef H2012_RESOURCEOBJECT_H
#define H2012_RESOURCEOBJECT_H

#include <QStandardItem>
#include <QMap>
#include <QString>

namespace Server
{
  class Client;
}


/**
  @brief The resource namespace holds the declaration of ResourceObjects, Lock and Detail
  */
namespace Resource
{
  /**
    @b Represents a resource with a list of features
    */
  class ResourceObject : public QStandardItem //should be scpi::resourcenode
  {

  public:
    /**
      @b Initialise the const variables
      @param name the name of the Resource::ResourceObject
      @param [in] *provider the provider of the resource
      @param description the description of the resource
      @param amount if the resource has a quantity that can be reserved partially it can specify the amount of that quantity
      @note Every Server::Resource object should only be adressed once
      */
    ResourceObject(QString name, Server::Client* provider, QString description, quint32 amount);

    /**
      @b Returns the name of the Resource::ResourceObject
      */
    const QString getName();
    /**
      @b Returns the provider of the Resource::ResourceObject
      */
    Server::Client* getProvider();
    /**
      @b Returns the description of the Resource::ResourceObject
      */
    const QString getDescription();
    /**
      @b Returns the amount of the Resource::ResourceObject
      */
    quint32 getAmount();
    /**
      @b Returns the free amount of the Resource::ResourceObject
      */
    quint32 getFreeAmount();
    /**
      @b Returns the occupiers of the Resource::ResourceObject
      */
    QList<Server::Client*> getOccupiers();

    /**
      @b occupies the resource
      @param occupier the client requesting the resource
      @param amount the amount of the resource requested
      @note the amount should be only >0 if the resource has a quantity (resourceAmount>0)
      @note the amount cannot be more than the resourceAmount
      @returns true if the occupation succeeded
      */
    bool occupyResource(Server::Client *occupier, quint32 amount=0);

    /**
      @b frees the resource
      @param occupier the client requesting the operation
      @param amount the amount of the resource that should be freed
      @note the amount should be only >0 if the resource has a quantity (resourceAmount>0)
      @note the amount cannot be more than the resourceAmount occupied by occupier
      @returns true if the occupation succeeded
      */
    bool freeResource(Server::Client *occupier, quint32 amount=0);
  private:
    /**
      @b name of the Resource::ResourceObject
      */
    const QString resourceName;
    /**
      @b provider of the Resource::ResourceObject
      */
    Server::Client* resourceProvider;
    /**
      @b amount of the Resource::ResourceObject
      @note if the amount is set to 0 then the resource can only be occupied once, and all occupy calls with amount>0 will fail with "bad amount"
      */
    const QString resourceDescription;
    /**
      @b free amount of the Resource::ResourceObject if the resource has  a quantity
      @note if a Server::Client occupies a resource and the resource has an amount > 0 the ResourceObject::Occupy call also needs to specify an amount > 0
      */
    const quint32 resourceAmount;
    /**
      @b description of the Resource::ResourceObject
      */
    quint32 freeAmount;
    /**
      @b occupiers of the Resource::ResourceObject
      @note if the resource has no quantity (amount=0) only one occupation is possible
      */
    QMap<Server::Client*, quint32> occupiers;
  };
}
#endif // H2012_RESOURCEOBJECT_H


