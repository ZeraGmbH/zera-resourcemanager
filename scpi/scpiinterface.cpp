#include "scpiinterface.h"
#include "server/client.h"
#include "resourceobject.h"
#include "resourcemanager.h"

#include <scpiobject.h>
#include <scpicommand.h>

#include <QStringList>

#include <QDebug>


namespace SCPI
{
  SCPIInterface::SCPIInterface(QObject* parent) : QObject(parent)
  {
    scpiInstance = new cSCPI("Resourcemanager");
    addResource=new Delegate("ADD", isCmdwP);
    removeResource=new Delegate("REMOVE", isCmdwP);
    catalogType=new Delegate("CATALOG", isCmd);

    //Add all default cSCPIObjects
    QStringList aTMP = QStringList("RESOURCE");
    scpiInstance->genSCPICmd(aTMP,addResource);
    scpiInstance->genSCPICmd(aTMP,removeResource);
    aTMP<<"TYPE";
    scpiInstance->genSCPICmd(aTMP,catalogType);


    connect(this,SIGNAL(resourceAdded(Application::Resource*)),this,SLOT(resourceAdd(Application::Resource*)));
  }

  SCPIInterface* SCPIInterface::singletonInstance=0;
  SCPIInterface* SCPIInterface::getInstance()
  {
    if(singletonInstance==0)
    {
      singletonInstance=new SCPIInterface;
    }
    return singletonInstance;
  }

  QStandardItemModel *SCPIInterface::getModel()
  {
    return scpiInstance->getSCPIModel();
  }

  QString SCPIInterface::listTypes()
  {
    /// @todo untested
    QString retVal;
    foreach(Catalog* tmpCat, catalogList)
    {
      if(!retVal.contains(tmpCat->getCatalogType()))
      {
        retVal.append(tmpCat->getCatalogType());
      }
    }

    return retVal;
  }

  QString SCPIInterface::listResourceByType(const QString &type)
  {
    return ResourceManager::getInstance()->listResources(type);
  }

  void SCPIInterface::resourceAdd(Application::Resource *res)
  {
    ResourceObject* newRes= new ResourceObject(res);
    QStringList position; //the scpi subsections where the resource will be recognized
    position<<"RESOURCE";
    position<<res->getType();

    //look for an existing catalog for the type res->getType()
    bool catalogFound=false;
    foreach(Catalog* tmpCat, catalogList)
    {
      if(tmpCat->getCatalogType()==res->getType())
      {
        tmpCat->upRef();//increment reference counter
        catalogFound=true;
        break;
      }
    }
    if(!catalogFound) //add a catalog to the type
    {
      Catalog* tmpCat= new Catalog();
      tmpCat->setName("CATALOG");
      tmpCat->setType(isCmd);//catalog is a command
      tmpCat->setCatalogType(res->getType());
      tmpCat->upRef();//increment reference counter
      catalogList.append(tmpCat);
      scpiInstance->genSCPICmd(position,tmpCat);
    }

    //add the resource to the scpi tree and our internal list
    newRes->setName(res->getName());
    newRes->setType(isQuery);
    resourceList.append(newRes);
    scpiInstance->genSCPICmd(position,newRes);
  }

  bool SCPIInterface::resourceRemove(Application::Resource *res, Server::Client *client)
  {
    bool retVal=false;
    if(client==res->getProvider())
    {
      ResourceManager::getInstance()->deleteResource(res);
      client->sendACK();
      retVal=true;
    }
    else
    {
      // this would be a strange error, someone tries to delete an existing resource he did not provide?#
      client->sendError(tr("Not owner of resource: %1").arg(res->getName()));
    }
    /// @todo .
    qDebug()<<"Removing resources is unimplemented";
    return retVal;
  }

  void SCPIInterface::resourceOccupy(Application::Resource *res, Server::Client *client, int amount)
  {
    if(res->occupyResource(client,amount))
    {
      client->sendACK();
    }
    else
    {
      client->sendNACK();
    }
  }

  void SCPIInterface::resourceFree(Application::Resource *res, Server::Client *client)
  {
    if(res->freeResource(client))
    {
      client->sendACK();
    }
    else
    {
      client->sendNACK();
    }
  }

  void SCPIInterface::scpiTransaction(const QString &commands)
  {
    Server::Client* currentClient=0;
    currentClient = static_cast<Server::Client*> (sender());
    if(currentClient!=0)
    {
      cSCPICommand command=commands;
      cSCPIObject* tmpObject = scpiInstance->getSCPIObject(command); //check what scpi node is triggered

      /// @todo remove debug code
      QString dbgString;
      for(quint32 i=0; i<command.getParamCount(); i++)
      {
        dbgString+=QString("%1;").arg(command.getParam(i));
      }
      qDebug()<<"Command:"<<command.getCommand()<<"Params:"<<command.getParamCount()<<dbgString<<endl;

      // check the delegates
      if(tmpObject==addResource) //A resource is about to be added
      {
        Application::Resource* tmpRes=0;
        tmpRes=ResourceManager::getInstance()->createResource(
              0,//command.getParam(AddParams::amount).toUInt(),
              command.getParam(CommandParams::description-1),/// @todo remove temporary workaround
              command.getParam(CommandParams::name),
              currentClient,
              command.getParam(CommandParams::type));
        emit resourceAdded(tmpRes);

      }
      else if(tmpObject==removeResource)
      {
        Application::Resource* tmpRes=0;
        tmpRes=ResourceManager::getInstance()->getResource(command.getParam(CommandParams::name),command.getParam(CommandParams::type));
        if(tmpRes!=0)
        {
          resourceRemove(tmpRes,currentClient);
        }
        else
        {
          currentClient->sendError(tr("Resource not found: %1").arg(command.getParam(CommandParams::name)));
        }
      }
      /// @todo return output to the client
    }
  }
}
