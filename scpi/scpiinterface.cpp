#include "scpiinterface.h"
#include "server/client.h"
#include "resourceobject.h"
#include "resourcemanager.h"

#include <scpiobject.h>
#include <scpicommand.h>

#include <QStringList>

namespace SCPI
{
  SCPIInterface::SCPIInterface(QObject* parent) : QObject(parent)
  {
    scpiInstance = new cSCPI("Resourcemanager");
    addResource=new Delegate("ADD", isCmdwP);
    catalogType=new Delegate("CATALOG", isCmd);
    removeResource=new Delegate("REMOVE", isCmdwP);
    resourceModel=new Delegate("MODEL",isQuery);
    resourceProvider=new Delegate("PROVIDER",isCmdwP);


    //Add all default cSCPIObjects
    QStringList aTMP = QStringList("RESOURCE");
    scpiInstance->genSCPICmd(aTMP,addResource);
    scpiInstance->genSCPICmd(aTMP,removeResource);
    scpiInstance->genSCPICmd(aTMP,resourceModel);
    scpiInstance->genSCPICmd(aTMP,resourceProvider);
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
    QString retVal;
    foreach(Catalog* tmpCat, catalogList)
    {
      if(!retVal.contains(tmpCat->getCatalogType()))
      {
        retVal.append(QString("%1;").arg(tmpCat->getCatalogType()));
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
    res->setObject(newRes);
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
      res->deleteLater();
      foreach(Catalog* tmpCat, catalogList)
      {
        if(tmpCat->getCatalogType()==res->getType())
        {
          tmpCat->unRef();
          if(tmpCat->getRefCount()==0)
          {
            catalogList.removeAll(tmpCat);
            scpiInstance->delSCPICmds(QString("RESOURCE:%1:CATALOG").arg(tmpCat->getCatalogType()));
            delete tmpCat;
          }
          break;
        }
      }
      retVal=true;
    }
    else
    {
      // this would be a strange error, someone tries to delete an existing resource he did not provide?#
      client->sendError(tr("Not owner of resource: %1").arg(res->getName()));
    }
    return retVal;
  }

  void SCPIInterface::scpiTransaction(const ProtobufMessage::NetMessage::ScpiCommand &pbSCPICommand)
  {
    bool retVal=false;
    Server::Client* currentClient=0;
    QString answer="";
    currentClient = static_cast<Server::Client*> (sender());
    if(currentClient!=0)
    {
      cSCPICommand command=QString("%1 %2").arg(QString::fromStdString(pbSCPICommand.command())).arg(QString::fromStdString(pbSCPICommand.parameter()));
      cSCPIObject* tmpObject=0;
      tmpObject=scpiInstance->getSCPIObject(command); //check which scpi node is triggered
      if(tmpObject!=0)
      {


        /// @todo remove debug code
        QString dbgString;
        for(quint32 i=0; i<command.getParamCount(); i++)
        {
          dbgString+=QString("%1;").arg(command.getParam(i));
        }
        answer=QString("Command: %1 Params(%2): %3").arg(command.getCommand()).arg(command.getParamCount()).arg(dbgString);
        qDebug()<<answer<<endl;


        // check the delegates
        if(tmpObject==addResource) //A resource is about to be added
        {
          Application::Resource* tmpRes=0;
          quint32 port=0;
          bool portConvert;
          if(command.getParamCount()==CommandParams::_paramcount)
          {
            port=command.getParam(CommandParams::port).toUInt(&portConvert);
            if(!portConvert) // check if the provider has a TCP/IP port where the resource is located
            {
              tmpRes=ResourceManager::getInstance()->createResource(
                    command.getParam(CommandParams::amount).toUInt(),
                    command.getParam(CommandParams::description),/// @todo remove temporary workaround
                    command.getParam(CommandParams::name),
                    currentClient,
                    command.getParam(CommandParams::type));
            }
            else
            {
              tmpRes=ResourceManager::getInstance()->createResource(
                    command.getParam(CommandParams::amount).toUInt(),
                    command.getParam(CommandParams::description),/// @todo remove temporary workaround
                    command.getParam(CommandParams::name),
                    currentClient,
                    command.getParam(CommandParams::type),
                    port);
            }
            emit resourceAdded(tmpRes);
            retVal=true;
          }
          retVal=false;
        }
        else if(tmpObject==removeResource)
        {
          QString tmpDelete=command.getParam(0); //get the first parameter
          cSCPICommand toDelete;
          Application::Resource* tmpRes=0;

          for(quint32 i=1; i<command.getParamCount(); i++)
          {
            tmpDelete=QString("%1:%2").arg(tmpDelete).arg(command.getParam(i));
          }
          qDebug()<<"Deleting with:  "<<tmpDelete;
          toDelete = tmpDelete;
          tmpRes=ResourceManager::getInstance()->getResourceByObject(static_cast<ResourceObject*>(scpiInstance->getSCPIObject(toDelete)));
          if(tmpRes!=0)
          {
            if(resourceRemove(tmpRes,currentClient))
            {
              scpiInstance->delSCPICmds(tmpDelete);
              retVal=true;
            }
          }
          else
          {
            answer=tr("Resource not found: %1").arg(command.getParam(CommandParams::name));
          }
        }
        else if(tmpObject==resourceModel) // return XMLized qstandarditem model
        {
          answer="";
          scpiInstance->exportSCPIModelXML(answer);
          retVal=true;
        }
        else if(tmpObject==resourceProvider) // return IP address and port
        {
          QString tmpSearch=command.getParam(0); //get the first parameter
          cSCPICommand searchCommand;
          Application::Resource* tmpRes=0;
          for(quint32 i=1; i<command.getParamCount(); i++)
          {
            tmpSearch=QString("%1:%2").arg(tmpSearch).arg(command.getParam(i));
          }

          searchCommand=tmpSearch;

          tmpRes=ResourceManager::getInstance()->getResourceByObject(static_cast<ResourceObject*>(scpiInstance->getSCPIObject(searchCommand)));
          if(tmpRes!=0)
          {
            retVal=true;
            answer=QString("%1;%2;").arg(tmpRes->getProvider()->getIpAdress()).arg(tmpRes->getPort());
          }

        }
        else if(tmpObject==catalogType)
        {
          answer=listTypes();
        }
        else // this is a true cSCPIObject try the standard routine
        {
          if(!tmpObject->executeSCPI(command.getCommand(),answer))
          {
            Application::Resource* tmpRes=ResourceManager::getInstance()->getResourceByObject(static_cast<ResourceObject*>(tmpObject));

            if(command.getParam(SetParams::command).toLower()==SetParams::SET_RESOURCE)
            {
              if(tmpRes->occupyResource(currentClient,command.getParam(SetParams::amount).toInt()))
              {
                retVal=true;
              }
            }
            else if(command.getParam(FreeParams::command).toLower()==FreeParams::FREE_RESOURCE)
            {
              if(tmpRes->freeResource(currentClient))
              {
                retVal=true;
              }
            }
          }
          else
          {
            retVal=true;
          }
        }


        if(retVal)
        {
          currentClient->sendACK(answer);
        }
        else
        {
          currentClient->sendNACK(answer);
        }


      }
      else //tmpObject was not found
      {
        currentClient->sendError(tr("ERROR INVALID SCPI COMMAND"));
      }
    }

  }
}
