#include "server/serverinterface.h"
#include "scpi/scpiinterface.h"
#include "resource/resource.h"
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
      Zera::Net::ZeraClient* client = static_cast<Zera::Net::ZeraClient*> (QObject::sender());

      qDebug()<<"Client disconnected:"<<client->getName();
      foreach(Client* cl, clients)
      {
        if(cl->isRepresenting(client))
        {
          clients.removeAll(cl);
          cl->deleteLater();
          break;
        }
      }
    }
  }

  void ServerInterface::lockFailed(Application::Resource *resource, Client *client)
  {
    client->sendNACK(tr("Failed to obtain resource: %1").arg(resource->getName()));
  }

  void ServerInterface::lockGranted(Application::Resource *resource, Client *client)
  {
    client->sendACK(tr("Access granted for resource: %1").arg(resource->getName()));
  }

  void ServerInterface::newClient(Zera::Net::ZeraClient* newClient)
  {
    Client* tmpClient = new Client(newClient);
    clients.append(tmpClient);
    connect(tmpClient,SIGNAL(aboutToDisconnect()),this,SLOT(clientDisconnected()));
  }


  ServerInterface::ServerInterface(QObject* parent) :
    QObject(parent),
    m_zServer(Zera::Net::ZeraServer::getInstance())
  {
    connect(m_zServer,SIGNAL(newClientAvailable(Zera::Net::ZeraClient*)),this,SLOT(newClient(Zera::Net::ZeraClient*)));
    m_zServer->startServer(12345); /// @todo Change port
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
