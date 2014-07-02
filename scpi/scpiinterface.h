#ifndef SCPIINTERFACE_H
#define SCPIINTERFACE_H

#include <QObject>
#include <QList>
#include <QStandardItemModel>

#include <netmessages.pb.h>

class cSCPI;

namespace Server
{
  class Client;
}


namespace Application
{
  class Resource;
}

/**
  @brief The SCPI namespace holds all the SCPI related classes and interfaces
  */
namespace SCPI
{
  class Catalog;
  class Delegate;
  /**
    @brief this encapsulates redundant entries for the add / remove parameters
    */
  namespace CommandParams
  {
    /**
      @brief This enum allows parameter reordering and will provide legacy support if changed
      */
    enum Parameters
    {
      type=0,
      name,
      amount,
      description,
      port,
      _paramcount
    };
  }

  namespace SetParams
  {
    const QString SET_RESOURCE="set";
    enum Parameters
    {
      command=0,
      amount
    };
  }

  namespace FreeParams
  {
    const QString FREE_RESOURCE="free";
    enum Parameters
    {
      command=0
    };
  }

  class ResourceObject;

  /**
    @brief The interface between this application and the SCPI library
    */
  class SCPIInterface : public QObject
  {
    Q_OBJECT
  protected:
    /**
      @brief default constructor, private due to Singleton
      */
    SCPIInterface(QObject* parent=0);

  public:
    /**
      @brief returns the singletonInstance
      */
    static SCPIInterface* getInstance();
    /**
      @brief For debugging, returns the cSCPI::model
      */
    QStandardItemModel* getModel();
    /**
      @brief uses the ResourceManager to list Application::Resource names of a given type
      */
    QString listResourceByType(const QString &type);
    /**
      @brief Lists types which have a Catalog command
      */
    QString listTypes();



  signals:
    /**
      @brief This signal will inform the Resourcemanager of new resources, it will also call SCPIInterface::resourceAdd()
      */
    void sigResourceAdded(Application::Resource* res);


  public slots:
    /**
      @brief The clients SCPI data is handled here
      */
    void onScpiTransaction(const ProtobufMessage::NetMessage::ScpiCommand &pbSCPICommand, QByteArray clientId);
    /**
      @brief Adds a SCPI::ResourceObject to the SCPI tree

      Also adds a SCPI::Catalog if the resource's type is a new type
      */
    void onResourceAdded(Application::Resource * res);
    /**
      @brief Removes a SCPI::ResourceObject from the SCPI tree

      @todo Also remove the SCPI::Catalog if the refcounter hits 0 and remove the node of the type
      */
    bool doResourceRemove(Application::Resource * res, Server::Client* client);

    /**
     * @brief resourceRemoveByProvider Deletes all resources the Server::Client provided
     *
     * Will be called when the Server::Client disconnects
     * @param client The provider of the resources
     */
    void doResourceRemoveByProvider(Server::Client* client);

    /**
     * @brief resourceRemoveByProvider Deletes all resources the Server::Client provided
     *
     * Will be called when the Server::Client disconnects
     * @param client The provider of the resources
     */
    void doResourceRemoveByProviderId(Server::Client *client, const QByteArray &providerId);

  private:
    /**
      @brief represents the RESOURCE:ADD in the SCPI tree
      */
    Delegate* m_addResource;
    /**
      @brief represents the RESOURCE:(TYPE):CATALOG in the SCPI tree
      */
    Delegate* m_catalogType;
    /**
      @brief represents the RESOURCE:REMOVE in the SCPI tree
      */
    Delegate* m_removeResource;

    /**
     * @brief Retrieves the XMLized model of the SCPI tree structure
     */
    Delegate* m_resourceModel;
    /**
      @brief Retrieves the service information for the resource
      */
    Delegate* m_resourceProvider;

    /**
      @brief SCPI library interaction is held here
      */
    cSCPI* m_scpiInstance;
    /**
      @brief Singleton instance, no other instances of this object are possible
      */
    static SCPIInterface* m_singletonInstance;

    /**
      @brief The list that holds the SCPI::Catalog for each catalogType
      */
    QList<Catalog *> m_catalogList;
    /**
      @brief Holds the cSCPIObjects for the tree
      */
    QList<ResourceObject *> m_resourceList;

    /**
      @brief Disables copying due to Singleton pattern
      */
    Q_DISABLE_COPY(SCPIInterface)
  };
}
#endif // SCPIINTERFACE_H
