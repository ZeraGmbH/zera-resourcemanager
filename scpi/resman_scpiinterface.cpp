#include "resman_scpiinterface.h"
#include "resman_catalog.h"
#include "resman_delegate.h"
#include "resourcemanager.h"
#include "resman_resourcescpiobject.h"
#include "resman_resource.h"
#include "resman_resourceidentity.h"
#include "resman_clientmultiton.h"

#include <scpiobject.h>
#include <scpicommand.h>

#include <QStringList>
#include <QDebug>

namespace SCPI
{
SCPIInterface::SCPIInterface(ResourceManager *t_resourceManager, QObject *t_parent) :
    QObject(t_parent),
    m_resourceManager(t_resourceManager)
{
    Q_ASSERT(m_resourceManager != nullptr);
    //Add all default cSCPIObjects
    QStringList aTMP = {{"RESOURCE"}};
    m_scpiInstance->insertScpiCmd(aTMP,m_addResource);
    m_scpiInstance->insertScpiCmd(aTMP,m_removeResource);
    m_scpiInstance->insertScpiCmd(aTMP,m_resourceModel);
    m_scpiInstance->insertScpiCmd(aTMP,m_resourceProvider);
    aTMP<<"TYPE";
    m_scpiInstance->insertScpiCmd(aTMP,m_catalogType);
}

QString SCPIInterface::listTypes() const
{
    QString retVal = getTypedCatalogHash().uniqueKeys().join(';');

    return retVal;
}

void SCPIInterface::removeSCPICommand(cSCPICommand t_command)
{
    m_scpiInstance->delSCPICmds(t_command);
}

void SCPIInterface::onScpiTransaction(ResourceServer::IClientMultiton *t_clientMultiton, const ProtobufMessage::NetMessage_ScpiCommand &t_pbSCPICommand)
{
    Q_ASSERT(t_clientMultiton != nullptr);

    QString answer="";
    const cSCPICommand command(QString("%1 %2").arg(QString::fromStdString(t_pbSCPICommand.command())).arg(QString::fromStdString(t_pbSCPICommand.parameter())));
    cSCPIObject *tmpObject=m_scpiInstance->getSCPIObject(command); //check which scpi node is triggered

    if(tmpObject!=nullptr)
    {
        bool retVal=false;
        /// @todo remove debug code
        answer=QString("Command: %2 Params(%3): %4;").arg(command.getCommand()).arg(command.getParamCount()).arg(command.getParamList().join(';'));
        qDebug() << QString("Client: '%1'").arg(t_clientMultiton->getName()) << "\n " <<
                    QString("Command: '%1'").arg(command.getCommand()) << "\n " <<
                    QString("Params: '%1'").arg(command.getParamList().join(';'));

        if(tmpObject==m_addResource) //A resource is about to be added
        {
            retVal = scpiAddResource(command, t_clientMultiton, answer);
        }
        else if(tmpObject==m_removeResource)
        {
            const QString resourceName = command.getParamList().last();
            const QList<Application::ResourceIdentity *> providedResourcesToRemove = m_resourceManager->getResourceIdentitiesOf<ResourceServer::IClientMultiton *>(t_clientMultiton);
            for(Application::ResourceIdentity *tmpResourceToRemove : providedResourcesToRemove)
            {
                if(resourceName == tmpResourceToRemove->getResource()->getName())
                {
                    qDebug() << "Removing resource: " << tmpResourceToRemove->getResource()->getName() << "on behalf of:" << t_clientMultiton->getName();
                    m_resourceManager->removeResourceIdentity(tmpResourceToRemove);
                    retVal = true;
                }
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
            Application::ResourceIdentity *resourceIdentity = m_resourceManager->getResourceIdentityOf<cSCPIObject *>(tmpObject);
            if(resourceIdentity != nullptr)
            {
                answer = QString("%1;%2;").arg(resourceIdentity->getProvider()->getIpAddress()).arg(resourceIdentity->getResource()->getPort());
                retVal = true;
            }
            else
            {
                answer=tr("Resource not found: %1").arg(command.getParam(CommandParams::name));
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
                Application::ResourceIdentity *resourceIdentity = m_resourceManager->getResourceIdentityOf<cSCPIObject *>(tmpObject);
                if(resourceIdentity != nullptr)
                {
                    if(command.getParam(SetParams::command).toLower()==SetParams::SET_RESOURCE)
                    {
                        retVal = (resourceIdentity->occupyResource(t_clientMultiton, command.getParam(SetParams::amount).toInt()) == true);
                        if(retVal == false)
                        {
#warning empty scpi answer here
                            //answer=tr("INSERT HERE");
                        }
                    }
                    else if(command.getParam(FreeParams::command).toLower()==FreeParams::FREE_RESOURCE)
                    {
                        retVal = (resourceIdentity->releaseResource(t_clientMultiton) == true);
                        if(retVal == false)
                        {
#warning empty scpi answer here
                            //answer=tr("INSERT HERE");
                        }
                    }
                }
            }
            else
            {
                retVal=true;
            }
        }

        if(retVal == true)
        {
            t_clientMultiton->doSendACK(answer);
        }
        else
        {
            t_clientMultiton->doSendNACK(answer);
        }
    }
    else //tmpObject was not found
    {
        t_clientMultiton->doSendError(tr("ERROR INVALID SCPI COMMAND"));
    }
}

void SCPIInterface::onClientMultitonDisconnected(ResourceServer::IClientMultiton *t_clientMultiton)
{
    Q_ASSERT(t_clientMultiton != nullptr);
    const QList<Application::ResourceIdentity *> occupiedResourcesToRelease = m_resourceManager->getOccupationsByClient(t_clientMultiton);
    for(Application::ResourceIdentity *tmpResourceToRelease : occupiedResourcesToRelease)
    {
        qDebug() << "Releasing resource: " << tmpResourceToRelease->getResource()->getName() << "on behalf of:" << t_clientMultiton->getName();
        tmpResourceToRelease->releaseResource(t_clientMultiton);
    }

    const QList<Application::ResourceIdentity *> providedResourcesToRemove = m_resourceManager->getResourceIdentitiesOf<ResourceServer::IClientMultiton *>(t_clientMultiton);
    for(Application::ResourceIdentity *tmpResourceToRemove : providedResourcesToRemove)
    {
        qDebug() << "Removing resource: " << tmpResourceToRemove->getResource()->getName() << "on behalf of:" << t_clientMultiton->getName();
        m_resourceManager->removeResourceIdentity(tmpResourceToRemove);
    }
}

bool SCPIInterface::scpiAddResource(cSCPICommand t_command, ResourceServer::IClientMultiton *t_clientMultiton, QString &t_answerOut)
{
    Q_ASSERT(t_clientMultiton != nullptr);

    bool retVal = false;
    if(t_command.getParamCount()==CommandParams::_paramcount)
    {
        quint16 port=0;
        bool portConvert;
        port=t_command.getParam(CommandParams::port).toUShort(&portConvert);
        if(portConvert) // check if the provider has a valid TCP/IP port where the resource is located
        {
            Application::Resource* newResource = new Application::Resource(t_command.getParam(CommandParams::amount).toUInt(),
                                                                           t_command.getParam(CommandParams::description),
                                                                           t_command.getParam(CommandParams::name),
                                                                           t_command.getParam(CommandParams::type),
                                                                           port);
            const QString resourceType = newResource->getType();
            //the scpi subsections where the resource will be recognized
            const QStringList scpiHierarchy { "RESOURCE", resourceType};
            ResourceSCPIObject* newResourceSCPIObject = new ResourceSCPIObject(newResource->getName(), isQuery);
            Catalog *resCatalog = getOrCreateResourceTypeCatalog(resourceType, scpiHierarchy);
            cSCPICommand tmpCommand(QString("%1:%2").arg(scpiHierarchy.join(':')).arg(newResource->getName()));
            Application::ResourceIdentity *resourceIdentity = new Application::ResourceIdentity(newResource, t_clientMultiton, resCatalog, newResourceSCPIObject, tmpCommand);

            newResourceSCPIObject->setResourceIdentity(resourceIdentity);

            m_resourceManager->addResourceIdentity(resourceIdentity);
            m_scpiInstance->insertScpiCmd(scpiHierarchy,newResourceSCPIObject);

            retVal=true;
        }
        else
        {
            t_answerOut=tr("Invalid parameter %1: expected 16 bit positive unsigned integer").arg(CommandParams::port+1);
        }
    }
    else
    {
        t_answerOut=tr("Invalid parameter count: %1 (expected: %2)").arg(t_command.getParamCount()).arg(CommandParams::_paramcount);
    }
    return retVal;
}

Catalog *SCPIInterface::getOrCreateResourceTypeCatalog(const QString &t_resourceType, const QStringList &t_scpiHierarchy)
{
    ///@note do not upref the catalog, since it is done in the Application::ResourceIdentity constructor
    Catalog *retVal = nullptr;
    //look for an existing catalog for the type res->getType()
    QHash<QString, Catalog *> resourceCatalogHash = getTypedCatalogHash();

    if(resourceCatalogHash.contains(t_resourceType))
    {
        retVal = resourceCatalogHash.value(t_resourceType);
    }
    else //add a new catalog for the type
    {
        Catalog* newCatalog= new Catalog(m_resourceManager, "CATALOG", isCmd);
        newCatalog->setCatalogType(t_resourceType);
        m_scpiInstance->insertScpiCmd(t_scpiHierarchy, newCatalog);
        retVal = newCatalog;
    }

    return retVal;
}

QHash<QString, Catalog *> SCPIInterface::getTypedCatalogHash() const
{
    QHash<QString, Catalog *> retVal;
    const QSet<Application::ResourceIdentity *> allResources = m_resourceManager->getResourceIdentitySet();
    QSetIterator<Application::ResourceIdentity *> setIterator(allResources);
    while(setIterator.hasNext())
    {
        Catalog *tmpCatalog = setIterator.next()->getCatalog();
        retVal.insert(tmpCatalog->getCatalogType(), tmpCatalog);
    }

    return retVal;
}
}
