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
  SCPI::SCPIInterface* scpiIface=0;
  Server::ServerInterface* sInterface=0;

  rManager=ResourceManager::getInstance();
  scpiIface=SCPI::SCPIInterface::getInstance();
  sInterface=Server::ServerInterface::getInstance();

  Q_UNUSED(rManager)
  Q_UNUSED(scpiIface)
  Q_UNUSED(sInterface)



  return a.exec();
}
