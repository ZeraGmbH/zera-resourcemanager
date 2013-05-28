#ifndef H2012_RESOURCEOBJECT_H
#define H2012_RESOURCEOBJECT_H

#include <QMap>
#include <QString>
#include <QObject>

namespace Server
{
  class Client;
}

namespace SCPI
{
  class ResourceObject;
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
      @brief Initialise the const variables
      @param amount if the resource has a quantity that can be reserved partially it can specify the amount of that quantity
      @param description the description of the resource
      @param name the name of the Application::Resource
      @param provider the provider socket id
      @param type the type of the resource
      @note Every Application::Resource object should only be adressed once
      */
    Resource(quint32 amount, const QString& description, const QString& name, Server::Client *provider, const QString& type, quint32 port=0 );

    /**
      @brief Returns the name of the Application::Resource
      */
    const QString &getName();
    /**
      @brief Returns the provider socket id
      */
    Server::Client *getProvider();
    /**
      @brief Returns the description of the Application::Resource
      */
    const QString &getDescription();
    /**
      @brief Returns the amount of the Application::Resource
      */
    quint32 getAmount();

    SCPI::ResourceObject* getResourceObject();
    /**
      @brief Returns the type of the Application::Resource
      */
    const QString &getType();
    /**
      @brief Returns the free amount of the Application::Resource
      */
    quint32 getFreeAmount();
    /**
      @brief Returns the occupiers of the Application::Resource
      */
    QList<Server::Client*> getOccupiers();

    quint32 getPort();

    /**
      @brief occupies the resource
      @param occupier the client requesting the resource
      @param amount the amount of the resource requested
      @note the amount should be only >0 if the resource has a quantity (resourceAmount>0)
      @note the amount cannot be more than the resourceAmount
      @returns true if the occupation succeeded
      */
    bool occupyResource(Server::Client* occupier, quint32 amount=0);

    /**
      @brief frees the resource
      @param occupier the client requesting the operation
      @returns true if succeeds
      */
    bool freeResource(Server::Client* occupier);

    void setObject(SCPI::ResourceObject* obj);
  private:
    /**
      @brief free amount of the SCPI::ResourceObject if the resource has  a quantity
      @note if a Server::Client occupies a resource and the resource has an amount > 0 the ResourceObject::Occupy call also needs to specify an amount > 0
      */
    quint32 freeAmount;

    /**
      @brief amount of the SCPI::ResourceObject
      @note if the amount is set to 0 then the resource can only be occupied once, and all occupy calls with amount>0 will fail with "bad amount"
      */
    const quint32 resourceAmount;

    /**
      @brief description of the SCPI::ResourceObject
      */
    const QString resourceDescription;

    /**
      @brief name of the SCPI::ResourceObject
      */
    const QString resourceName;

    SCPI::ResourceObject* resourceObject;

    /**
     * @brief The port where the resource can be accessed
     */
    quint32 resourcePort;

    /**
      @brief socket id of the provider of this SCPI::ResourceObject
      */
    Server::Client * resourceProvider;

    /**
      @brief type of the SCPI::ResourceObject, e.g. Sense for sensors
      */
    const QString resourceType;

    /**
      @brief occupiers (socket ids) of the SCPI::ResourceObject, quint32 is the amount
      @note if the resource has no quantity (amount=0) only one occupation is possible
      */
    QMap<Server::Client*, quint32> occupiers;
  };
}
#endif // H2012_RESOURCEOBJECT_H


