#include "server/serverinterface.h"
#include "scpi/scpiinterface.h"
#include "resource/resource.h"
#include <QDebug>

namespace Server
{
  ServerInterface::ServerInterface(QObject* parent) :
    QTcpServer(parent), thread(new QThread(this))
  {

    this->listen(QHostAddress::Any, 12345);
    qDebug()<<"Server Started";
  }

  ServerInterface::~ServerInterface()
  {
    thread->deleteLater();
    foreach(Client *c, clients)
    {
      c->deleteLater();
    }
  }

  void ServerInterface::incomingConnection(int socketDescriptor)
  {
    qDebug()<<"Client connected";

    Client *client = new Client(socketDescriptor);
    clients.append(client);
    client->moveToThread(thread);
    connect(client,SIGNAL(),SCPI::SCPIInterface::getInstance(),SLOT(scpiTransaction(QString)));
    connect(client, SIGNAL(clientFinished()), this, SLOT(clientDisconnect()));
    if(!thread->isRunning())
    {
      thread->start();
    }
    client->run();
  }

  void ServerInterface::clientDisconnect()
  {
    if(QObject::sender()!=0)
    {
      Client* client = (Client*) QObject::sender();
      qDebug()<<"Client disconnected:"<<client->getName();
      clients.removeAll(client);
      client->deleteLater();
    }
  }

  void ServerInterface::lockFailed(Application::Resource *resource, Client *client)
  {
    client->sendToClient("Lock failed for:"+resource->getName());
  }

  void ServerInterface::lockGranted(Application::Resource *resource, Client *client)
  {
    client->sendToClient("Lock granted for:"+resource->getName());
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
