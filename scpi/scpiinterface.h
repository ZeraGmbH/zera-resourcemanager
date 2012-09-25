#ifndef SCPIINTERFACE_H
#define SCPIINTERFACE_H

#include <QObject>
#include <QMutex>
#include <QList>
#include <scpi.h>

#include "delegate.h"
#include "catalog.h"

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
  /**
    @b this encapsulates redundant entries for the add / remove parameters
    */
  namespace AddParams
  {
    enum Parameters
    {
      type=0,
      name,
      amount,
      description,
      providerSocketDesc
    };
  }
  /**
    @b this encapsulates redundant entries for the add / remove parameters
    */
  namespace RemoveParams
  {
    enum Parameters
    {
      type=0,
      name,
      providerSocketDesc
    };
  }

  /**
    @b this will encapsulate redundant entries for the add / remove parameters
    */
  namespace OccupyParams
  {
    enum Parameters
    {
      amount=0,
      providerSocketDesc
    };
  }
  /**
    @b this will encapsulate redundant entries for the add / remove parameters
    */
  namespace FreeParams
  {
    enum Parameters
    {
      providerSocketDesc=0
    };
  }

  class ResourceObject;

  class SCPIInterface : public QObject
  {
    Q_OBJECT
  protected:
    SCPIInterface(QObject* parent=0);

  public:
    static SCPIInterface* getInstance();
    QStandardItemModel* getModel();



  signals:
    void resourceAdded(Application::Resource* res);


  public slots:
    void scpiTransaction(QString commands);
    void resourceAdd(Application::Resource * res);
    void resourceRemove(Application::Resource * res, int clientSockedDesc);
    void resourceOccupy(Application::Resource * res, int clientSockedDesc, int amount);
    void resourceFree(Application::Resource * res, int clientSockedDesc, int amount);

  private:
    Delegate* addResource;
    Delegate* removeResource;
    Delegate* catalogType;
    cSCPI* scpiInstance;
    static SCPIInterface* singletonInstance;

    QMutex scpiMutex;

    QList<Catalog *> catalogList;
    QList<ResourceObject *> resourceList;


    Q_DISABLE_COPY(SCPIInterface)
  };
}
#endif // SCPIINTERFACE_H
