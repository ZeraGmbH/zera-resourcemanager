#include <QApplication>

#include "server/serverinterface.h"
#include "resourcemanager.h"
#include "scpi/scpiinterface.h"

//debug
#include "resource/resource.h"
#include "server/client.h"
#include <QDebug>
#include <QTreeView>

#include <QInputContextFactory>

int main(int argc, char* argv[])
{
  QApplication a(argc,argv);

  ResourceManager* rManager=0;
  rManager=ResourceManager::getInstance();

  SCPI::SCPIInterface* scpiIface=0;
  scpiIface=SCPI::SCPIInterface::getInstance();

  Server::ServerInterface* sInterface=0;
  sInterface=Server::ServerInterface::getInstance();


  return a.exec();
}
