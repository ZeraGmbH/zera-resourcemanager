#include "server/client.h"

namespace Server
{
  Client::Client(QString clientName, QObject *parent, QTcpSocket *clientSocket) :
    QObject(parent), name(clientName), socket(clientSocket)
  {
    connect(&timeoutCheck, SIGNAL(timeout()), this,SIGNAL(timeout()));
    /** set the timer to 5 seconds */
    timeoutCheck.start(5000);
  }

  QString Client::getName()
  {
    return name;
  }

  const QTcpSocket *Client::getSocket()
  {
    return socket;
  }

  void Client::refresh()
  {
    timeoutCheck.start(5000);
  }

  void Client::lockFailed(Resource::ResourceObject *res)
  {
  }
}
