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

#define FULL_DEBUG

int main(int argc, char* argv[])
{
  QApplication a(argc,argv);

  ResourceManager* rManager=0;
  rManager=ResourceManager::getInstance();

  Server::ServerInterface* sInterface=0;
  sInterface=Server::ServerInterface::getInstance();

  SCPI::SCPIInterface* scpiIface=0;
  scpiIface=SCPI::SCPIInterface::getInstance();

#ifdef FULL_DEBUG
  QTreeView view;
  view.setModel(scpiIface->getModel());
  view.setGeometry(view.x(),view.y(), 480, 576);
  view.show();
  view.setSelectionBehavior(QAbstractItemView::SelectRows);


  Server::Client* testClient=new Server::Client(0);

  QObject::connect(testClient,SIGNAL(scpiCommandSent(QString)),scpiIface,SLOT(scpiTransaction(QString)));

  testClient->testScpiCommand("RESOURCE:ADD SENSE;UL1;;Messkanal 0V..480V;");
  testClient->testScpiCommand("RESOURCE:ADD SENSE;UL2;;Messkanal 0V..480V;");
  testClient->testScpiCommand("RESOURCE:ADD MMEMORY;DSP1PGRCMEM;1024;DSP1 ProgramMemory (Cyclic);");
  testClient->testScpiCommand("RESOURCE:REMOVE SENSE;UL1;");
#endif





  return a.exec();
}
