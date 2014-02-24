#include "server/serverinterface.h"
#include "scpi/scpiinterface.h"
#include "resource/resource.h"
#include "server/client.h"

#include "rmprotobufwrapper.h"

#include <protonetserver.h>


#include <QDebug>


namespace Server
{
  ServerInterface* ServerInterface::getInstance()
  {
    if(singletonInstance==0)
    {
      singletonInstance=new ServerInterface;
    }
    return singletonInstance;
  }

  void ServerInterface::clientDisconnected()
  {
    if(QObject::sender()!=0)
    {
      Client* cl = qobject_cast<Server::Client*> (QObject::sender());

      qDebug()<<"Client disconnected:"<<cl->getName();

      SCPI::SCPIInterface::getInstance()->resourceRemoveByProvider(cl);

      clients.removeAll(cl);
      cl->deleteLater();
    }
  }

  void ServerInterface::newClient(ProtoNetPeer *zcl)
  {
    Client* tmpClient = new Client(zcl);
    clients.append(tmpClient);
    connect(tmpClient,SIGNAL(aboutToDisconnect()),this,SLOT(clientDisconnected()));
    connect(tmpClient,SIGNAL(scpiCommandSent(ProtobufMessage::NetMessage::ScpiCommand)),SCPI::SCPIInterface::getInstance(),SLOT(scpiTransaction(ProtobufMessage::NetMessage::ScpiCommand)));
  }

  ServerInterface::ServerInterface(QObject* parent) :
    QObject(parent),
    m_zServer(new ProtoNetServer(this)),
    m_defaultWrapper(new RMProtobufWrapper())
  {
    m_zServer->setDefaultWrapper(m_defaultWrapper);
    connect(m_zServer,SIGNAL(sigClientConnected(ProtoNetPeer*)),this,SLOT(newClient(ProtoNetPeer*)));
    m_zServer->startServer(6312); /// @todo Change port
  }

  ServerInterface::~ServerInterface()
  {
    foreach(Client *c, clients)
    {
      c->deleteLater();
    }
  }
  ServerInterface* ServerInterface::singletonInstance=0;

}
