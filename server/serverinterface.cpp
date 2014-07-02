#include "server/serverinterface.h"
#include "scpi/scpiinterface.h"
#include "resource/resource.h"
#include "server/client.h"

#include "rmprotobufwrapper.h"

#include <protonetserver.h>
#include <protonetpeer.h>


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

      SCPI::SCPIInterface::getInstance()->doResourceRemoveByProvider(cl);

      clients.removeAll(cl);
      cl->deleteLater();
    }
  }

  void ServerInterface::newClient(ProtoNetPeer *zcl)
  {
    Client* tmpClient = new Client(zcl);
    clients.append(tmpClient);
    connect(tmpClient, &Client::sigAboutToDisconnect, this, &ServerInterface::clientDisconnected);
    connect(tmpClient, &Client::sigScpiTransaction, SCPI::SCPIInterface::getInstance(), &SCPI::SCPIInterface::onScpiTransaction);
  }

  ServerInterface::ServerInterface(QObject* parent) :
    QObject(parent),
    m_zServer(new ProtoNetServer(this)),
    m_defaultWrapper(new RMProtobufWrapper())
  {
    m_zServer->setDefaultWrapper(m_defaultWrapper);
    connect(m_zServer, &ProtoNetServer::sigClientConnected,this, &ServerInterface::newClient);
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
