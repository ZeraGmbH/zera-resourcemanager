#include "catalog.h"
#include "delegate.h"
#include "resourcemanager.h"
#include "resourceobject.h"
#include "resource/resource.h"
#include "scpiinterface.h"
#include "server/clientmultiton.h"


#include <scpi.h>
#include <scpiobject.h>
#include <scpicommand.h>

#include <QStringList>
#include <QDebug>

namespace SCPI
{
  SCPIInterface::SCPIInterface(QObject* parent) : QObject(parent)
  {
    m_scpiInstance = new cSCPI("Resourcemanager");
    m_addResource=new Delegate("ADD", isCmdwP);
    m_catalogType=new Delegate("CATALOG", isCmd);
    m_removeResource=new Delegate("REMOVE", isCmdwP);
    m_resourceModel=new Delegate("MODEL",isQuery);
    m_resourceProvider=new Delegate("PROVIDER",isCmdwP);


    //Add all default cSCPIObjects
    QStringList aTMP = QStringList("RESOURCE");
    m_scpiInstance->genSCPICmd(aTMP,m_addResource);
    m_scpiInstance->genSCPICmd(aTMP,m_removeResource);
    m_scpiInstance->genSCPICmd(aTMP,m_resourceModel);
    m_scpiInstance->genSCPICmd(aTMP,m_resourceProvider);
    aTMP<<"TYPE";
    m_scpiInstance->genSCPICmd(aTMP,m_catalogType);

    connect(this, &SCPIInterface::sigResourceAdded,this, &SCPIInterface::onResourceAdded);
  }

  SCPIInterface* SCPIInterface::m_singletonInstance=0;
  SCPIInterface* SCPIInterface::getInstance()
  {
    if(m_singletonInstance==0)
    {
      m_singletonInstance=new SCPIInterface;
    }
    return m_singletonInstance;
  }

  QStandardItemModel *SCPIInterface::getModel()
  {
    return m_scpiInstance->getSCPIModel();
  }

  QString SCPIInterface::listTypes()
  {
    QString retVal;
    foreach(Catalog* tmpCat, m_catalogList)
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

  void SCPIInterface::onResourceAdded(Application::Resource *res)
  {
    ResourceObject* newRes= new ResourceObject(res);
    res->setObject(newRes);
    QStringList position; //the scpi subsections where the resource will be recognized
    position<<"RESOURCE";
    position<<res->getType();

    //look for an existing catalog for the type res->getType()
    bool catalogFound=false;
    foreach(Catalog* tmpCat, m_catalogList)
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
      m_catalogList.append(tmpCat);
      m_scpiInstance->genSCPICmd(position,tmpCat);
    }

    //add the resource to the scpi tree and our internal list
    newRes->setName(res->getName());
    newRes->setType(isQuery);
    m_resourceList.append(newRes);
    m_scpiInstance->genSCPICmd(position,newRes);
  }

  bool SCPIInterface::doResourceRemove(Application::Resource *res, Server::ClientMultiton *client)
  {
    bool retVal=false;
    if(client==res->getProvider())
    {
      ResourceManager::getInstance()->onResourceRemoved(res);
      res->deleteLater();
      m_scpiInstance->delSCPICmds(QString("RESOURCE:%1:%2").arg(res->getType()).arg(res->getName()));
      delete res->getResourceObject();
      foreach(Catalog* tmpCat, m_catalogList)
      {
        if(tmpCat->getCatalogType()==res->getType())
        {
          tmpCat->unRef();
          if(tmpCat->getRefCount()==0)
          {
            m_catalogList.removeAll(tmpCat);
            m_scpiInstance->delSCPICmds(QString("RESOURCE:%1:CATALOG").arg(tmpCat->getCatalogType()));
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
      client->doSendError(tr("Not owner of resource: %1").arg(res->getName()));
    }
    return retVal;
  }

  void SCPIInterface::doResourceRemoveByProvider(Server::ClientMultiton *client)
  {
    ResourceObject *resObj;
    Application::Resource* res;
    foreach(resObj, m_resourceList)
    {
      res = ResourceManager::getInstance()->getResourceByObject(resObj);
      if(res!=0 && res->getProvider()==client)
      {
        doResourceRemove(res, client);
        qDebug() << "Deleting" << resObj->getName();
        delete res;
      }
    }
  }

  void SCPIInterface::onScpiTransaction(const ProtobufMessage::NetMessage::ScpiCommand &pbSCPICommand)
  {
    bool retVal=false;
    Server::ClientMultiton* currentClient=0;
    QString answer="";
    currentClient = qobject_cast<Server::ClientMultiton*> (sender());
    if(currentClient!=0)
    {
      cSCPICommand command=QString("%1 %2").arg(QString::fromStdString(pbSCPICommand.command())).arg(QString::fromStdString(pbSCPICommand.parameter()));
      cSCPIObject* tmpObject=0;
      tmpObject=m_scpiInstance->getSCPIObject(command); //check which scpi node is triggered
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
        if(tmpObject==m_addResource) //A resource is about to be added
        {
          if(command.getParamCount()==CommandParams::_paramcount)
          {
            Application::Resource* tmpRes=0;
            quint16 port=0;
            bool portConvert;
            port=command.getParam(CommandParams::port).toUShort(&portConvert);
            if(portConvert) // check if the provider has a valid TCP/IP port where the resource is located
            {
              tmpRes=ResourceManager::getInstance()->createResource(
                    command.getParam(CommandParams::amount).toUInt(),
                    command.getParam(CommandParams::description),
                    command.getParam(CommandParams::name),
                    currentClient,
                    command.getParam(CommandParams::type),
                    port);
              emit sigResourceAdded(tmpRes);
              retVal=true;
            }
            else
            {
              answer=tr("Invalid parameter %1: expected 16 bit positive unsigned integer").arg(CommandParams::port+1);
            }
          }
          else
          {
            answer=tr("Invalid parameter count: %1 (expected: %2)").arg(command.getParamCount()).arg(CommandParams::_paramcount);
          }
        }
        else if(tmpObject==m_removeResource)
        {
          QString tmpDelete=command.getParam(0); //get the first parameter
          cSCPICommand toDelete;
          Application::Resource* tmpRes=0;

          for(quint32 i=1; i<command.getParamCount(); i++)
          {
            tmpDelete=QString("%1:%2").arg(tmpDelete).arg(command.getParam(i));
          }
          qDebug()<<"Deleting with:  "<<tmpDelete;
          toDelete = QString("RESOURCE:%1").arg(tmpDelete);
          tmpRes=ResourceManager::getInstance()->getResourceByObject(static_cast<ResourceObject*>(m_scpiInstance->getSCPIObject(toDelete)));
          if(tmpRes!=0)
          {
            if(doResourceRemove(tmpRes,currentClient))
            {
              m_scpiInstance->delSCPICmds(tmpDelete);
              retVal=true;
            }
          }
          else
          {
            answer=tr("Resource not found: %1").arg(command.getParam(CommandParams::name));
          }
        }
        else if(tmpObject==m_resourceModel) // return XMLized qstandarditem model
        {
          answer="";
          m_scpiInstance->exportSCPIModelXML(answer);
          retVal=true;
        }
        else if(tmpObject==m_resourceProvider) // return IP address and port
        {
          QString tmpSearch=command.getParam(0); //get the first parameter
          cSCPICommand searchCommand;
          Application::Resource* tmpRes=0;
          for(quint32 i=1; i<command.getParamCount(); i++)
          {
            tmpSearch=QString("%1:%2").arg(tmpSearch).arg(command.getParam(i));
          }

          searchCommand=tmpSearch;

          tmpRes=ResourceManager::getInstance()->getResourceByObject(static_cast<ResourceObject*>(m_scpiInstance->getSCPIObject(searchCommand)));
          if(tmpRes!=0)
          {
            retVal=true;
            answer=QString("%1;%2;").arg(tmpRes->getProvider()->getIpAddress()).arg(tmpRes->getPort());
          }

        }
        else if(tmpObject==m_catalogType)
        {
          answer=listTypes();
          retVal=true;
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
          currentClient->doSendACK(answer);
        }
        else
        {
          currentClient->doSendNACK(answer);
        }


      }
      else //tmpObject was not found
      {
        currentClient->doSendError(tr("ERROR INVALID SCPI COMMAND"));
      }
    }

  }
}
