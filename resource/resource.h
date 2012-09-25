#ifndef H2012_RESOURCEOBJECT_H
#define H2012_RESOURCEOBJECT_H

#include <QMap>
#include <QString>
#include <QObject>

namespace Server
{
  class Client;
}


/**
  @brief The Application namespace holds the application specific informations (e.g. Resources)
  */
namespace Application
{
  /**
    @b Represents a resource with a list of features
    */
  class Resource : public QObject
  {
  Q_OBJECT
  public:
    /**
      @b Initialise the const variables
      @param name the name of the Application::ResourceObject
      @param provider the provider socket id
      @param description the description of the resource
      @param amount if the resource has a quantity that can be reserved partially it can specify the amount of that quantity
      @note Every Server::Resource object should only be adressed once
      */
    Resource(quint32 amount, const QString& description, const QString& name, int provider, const QString& type );

    /**
      @b Returns the name of the Application::ResourceObject
      */
    const QString getName();
    /**
      @b Returns the provider socket id
      */
    int getProvider();
    /**
      @b Returns the description of the Application::ResourceObject
      */
    const QString getDescription();
    /**
      @b Returns the amount of the Application::ResourceObject
      */
    quint32 getAmount();

    QString getType();
    /**
      @b Returns the free amount of the Application::ResourceObject
      */
    quint32 getFreeAmount();
    /**
      @b Returns the occupiers of the Application::ResourceObject
      */
    QList<int> getOccupiers();

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
      @param amount the amount of the resource that should be freed
      @note the amount should be only >0 if the resource has a quantity (resourceAmount>0)
      @note the amount cannot be more than the resourceAmount occupied by occupier
      @returns true if succeeds
      */
    bool freeResource(Server::Client* occupier, quint32 amount=0);
  private:
    /**
      @b name of the Application::ResourceObject
      */
    const QString resourceName;
    /**
      @b socket id of the provider of this Application::ResourceObject
      */
    int resourceProvider;

    /**
      @b type of the Application::ResourceObject, e.g. Sense for sensors
      */
    const QString resourceType;

    /**
      @b amount of the Application::ResourceObject
      @note if the amount is set to 0 then the resource can only be occupied once, and all occupy calls with amount>0 will fail with "bad amount"
      */
    const QString resourceDescription;
    /**
      @b free amount of the Application::ResourceObject if the resource has  a quantity
      @note if a Server::Client occupies a resource and the resource has an amount > 0 the ResourceObject::Occupy call also needs to specify an amount > 0
      */
    const quint32 resourceAmount;

    /**
      @b description of the Application::ResourceObject
      */
    quint32 freeAmount;
    /**
      @b occupiers (socket ids) of the Application::ResourceObject
      @note if the resource has no quantity (amount=0) only one occupation is possible
      */
    QMap<int, quint32> occupiers;
  };
}
#endif // H2012_RESOURCEOBJECT_H


