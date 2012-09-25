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
    addResource=new Delegate("ADD", SCPI::isCmdwP);
    removeResource=new Delegate("REMOVE", SCPI::isCmdwP);
    catalogType=new Delegate("CATALOG", SCPI::isCmd);

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

  void SCPIInterface::resourceRemove(Application::Resource *res, int clientSockedDesc)
  {
  }

  void SCPIInterface::resourceOccupy(Application::Resource *res, int clientSockedDesc, int amount)
  {
  }

  void SCPIInterface::resourceFree(Application::Resource *res, int clientSockedDesc, int amount)
  {
  }

  void SCPIInterface::scpiTransaction(QString commands)
  {
    scpiMutex.lock();
    Server::Client* currentClient=0;
    currentClient = (Server::Client*) sender();
    if(currentClient!=0)
    {
      QString socket=QString("%1;").arg(currentClient->getSocket());
      cSCPICommand command=commands+socket; //Add the clients socket descriptor to the scpi commands
      cSCPIObject* tmpObject = scpiInstance->getSCPIObject(command); //check what scpi node is triggered
      if(tmpObject==addResource) //A resource is about to be added
      {
        Application::Resource* tmpRes=0;
        tmpRes=ResourceManager::getInstance()->createResource(
              0,//command.getParam(AddParams::amount).toUInt(),
              command.getParam(AddParams::description-1),/// @todo remove temporary workaround
              command.getParam(AddParams::name),
              command.getParam(AddParams::providerSocketDesc-1).toInt(), /// @todo remove temporary workaround
              command.getParam(AddParams::type));
        emit resourceAdded(tmpRes);

        /// @todo remove debug code
        QString dbgString;
        for(int i=0; i<command.getParamCount(); i++)
        {
          dbgString+=QString("%1;").arg(command.getParam(i));
        }
        qDebug()<<"Command:"<<command.getCommand()<<"Params:"<<command.getParamCount()<<dbgString<<endl;
      }
      /// @todo return output to the client
    }
    scpiMutex.unlock();
  }
}
