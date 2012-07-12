#include "server/serverinterface.h"
#include <QDebug>

namespace Server
{
  ServerInterface::ServerInterface(QObject* parent) :
    QObject(parent)
  {
  }

  void ServerInterface::newClient(Client* client)
  {
    clients.append(client);
  }

  void ServerInterface::disconnected(Client* client)
  {
    clients.removeAll(client);
  }

  void ServerInterface::lockFailed(Resource::ResourceObject *resource, Client *client)
  {
    //qDebug()<<"Lock failed for resource:" << QObject(resource).objectName() << "requested by client: "<< client->getName();
    client->lockFailed(resource);
  }

  ServerInterface* ServerInterface::singletonInstance=0;

  ServerInterface* ServerInterface::getInstance()
  {
    if(singletonInstance==0)
    {
      singletonInstance=new ServerInterface;
    }
    return singletonInstance;
  }
}
