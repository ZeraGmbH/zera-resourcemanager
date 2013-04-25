#ifndef H2012_RESOURCEOBJECT_H
#define H2012_RESOURCEOBJECT_H

#include <QMap>
#include <QString>
#include <QObject>

namespace Server
{
  class Client;
}

namespace Zera
{
  namespace Net
  {
    class ZeraClient;
  }
}


/**
  @brief The Application namespace holds the application specific informations (e.g. Resources)
  */
namespace Application
{
  /**
    @brief Represents a resource with a list of features
    */
  class Resource : public QObject
  {
    Q_OBJECT
  public:
    /**
      @b Initialise the const variables
      @param amount if the resource has a quantity that can be reserved partially it can specify the amount of that quantity
      @param description the description of the resource
      @param name the name of the Application::Resource
      @param provider the provider socket id
      @param type the type of the resource
      @note Every Application::Resource object should only be adressed once
      */
    Resource(quint32 amount, const QString& description, const QString& name, Server::Client *provider, const QString& type );

    /**
      @b Returns the name of the Application::Resource
      */
    const QString &getName();
    /**
      @b Returns the provider socket id
      */
    Server::Client *getProvider();
    /**
      @b Returns the description of the Application::Resource
      */
    const QString &getDescription();
    /**
      @b Returns the amount of the Application::Resource
      */
    quint32 getAmount();

    /**
      @b Returns the type of the Application::Resource
      */
    const QString &getType();
    /**
      @b Returns the free amount of the Application::Resource
      */
    quint32 getFreeAmount();
    /**
      @b Returns the occupiers of the Application::Resource
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
    bool occupyResource(Server::Client* occupier, quint32 amount=0);

    /**
      @b frees the resource
      @param occupier the client requesting the operation
      @returns true if succeeds
      */
    bool freeResource(Server::Client* occupier);
  private:
    /**
      @b free amount of the SCPI::ResourceObject if the resource has  a quantity
      @note if a Server::Client occupies a resource and the resource has an amount > 0 the ResourceObject::Occupy call also needs to specify an amount > 0
      */
    quint32 freeAmount;

    /**
      @b amount of the SCPI::ResourceObject
      @note if the amount is set to 0 then the resource can only be occupied once, and all occupy calls with amount>0 will fail with "bad amount"
      */
    const quint32 resourceAmount;

    /**
      @b description of the SCPI::ResourceObject
      */
    const QString resourceDescription;

    /**
      @b name of the SCPI::ResourceObject
      */
    const QString resourceName;

    /**
      @b socket id of the provider of this SCPI::ResourceObject
      */
    Server::Client * resourceProvider;

    /**
      @b type of the SCPI::ResourceObject, e.g. Sense for sensors
      */
    const QString resourceType;

    /**
      @b occupiers (socket ids) of the SCPI::ResourceObject, quint32 is the amount
      @note if the resource has no quantity (amount=0) only one occupation is possible
      */
    QMap<Server::Client*, quint32> occupiers;
  };
}
#endif // H2012_RESOURCEOBJECT_H


