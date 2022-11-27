#ifndef H2012_RESOURCEOBJECT_H
#define H2012_RESOURCEOBJECT_H

#include <QString>

/**
  @brief The Application namespace holds the application specific informations (e.g. Resources)
  */
namespace Application
{
/**
  @brief Represents a resource with a list of features
  */
class Resource
{
public:
    /**
      @brief Initialise the const variables
      @param t_amount if the resource has a quantity that can be reserved partially it can specify the amount of that quantity
      @param t_description the description of the resource
      @param t_name the name of the Application::Resource
      @param t_provider the provider socket id
      @param t_type the type of the resource
      @param t_port the network port
      @note Every Application::Resource object should only be adressed once
      */
    Resource(quint32 t_amount, const QString& t_description, const QString& t_name, const QString& t_type, quint16 t_port);
    /**
      @brief Returns the name of the Application::Resource
      */
    const QString &getName() const;
    /**
      @brief Returns the description of the Application::Resource
      */
    const QString &getDescription() const;
    /**
      @brief Returns the amount of the Application::Resource
      */
    quint32 getAmount() const;

    /**
      @brief Returns the type of the Application::Resource
      */
    const QString &getType() const;

    /**
     * @brief getPort Returns the service port where the resource is located
     */
    quint16 getPort() const;

private:

    /**
      @brief amount of the SCPI::ResourceObject
      @note if the amount is set to 0 then the resource can only be occupied once, and all occupy calls with amount>0 will fail with "bad amount"
      */
    const quint32 m_resourceAmount;

    /**
      @brief description of the SCPI::ResourceObject
      */
    const QString m_resourceDescription;

    /**
      @brief name of the SCPI::ResourceObject
      */
    const QString m_resourceName;

    /**
     * @brief The port where the resource can be accessed
     */
    const quint16 m_resourcePort;

    /**
      @brief type of the SCPI::ResourceObject, e.g. Sense for sensors
      */
    const QString m_resourceType;
};
}
#endif // H2012_RESOURCEOBJECT_H


