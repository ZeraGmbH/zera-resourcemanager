#include "server/serverinterface.h"
#include "scpi/scpiinterface.h"
#include "resource/resource.h"
#include <QDebug>

namespace Server
{
  ServerInterface::ServerInterface(QObject* parent) :
    QTcpServer(parent), netThread(new QThread(this))
  {

    /// @todo change default port
    this->listen(QHostAddress::Any, 12345);
    qDebug()<<"Server Started";
  }

  ServerInterface::~ServerInterface()
  {
    netThread->deleteLater();
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
    client->moveToThread(netThread); //the client will be executed in the QThread
    connect(client,SIGNAL(),SCPI::SCPIInterface::getInstance(),SLOT(scpiTransaction(QString)));
    connect(client, SIGNAL(clientFinished()), this, SLOT(clientDisconnect()));
    /// @note There might be cases where the thread is not running because all clients finished previously
    if(!netThread->isRunning())
    {
      netThread->start();
    }
    client->run();
  }

  void ServerInterface::clientDisconnect()
  {
    if(QObject::sender()!=0)
    {
      Client* client = static_cast<Client*> (QObject::sender());
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
