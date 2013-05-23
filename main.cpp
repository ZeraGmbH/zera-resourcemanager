#include <QApplication>
#include <QTextCodec>
#include "server/serverinterface.h"
#include "resourcemanager.h"
#include "scpi/scpiinterface.h"

int main(int argc, char* argv[])
{
  QApplication a(argc,argv);

  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // Protobuf does not like latin-1

  ResourceManager* rManager=0;
  SCPI::SCPIInterface* scpiIface=0;
  Server::ServerInterface* sInterface=0;

  rManager=ResourceManager::getInstance();
  scpiIface=SCPI::SCPIInterface::getInstance();
  sInterface=Server::ServerInterface::getInstance();

  // we only need to call the constructor of these, the rest is handled by the qt even queue, but gcc thinks the variables are unused objects
  Q_UNUSED(rManager)
  Q_UNUSED(scpiIface)
  Q_UNUSED(sInterface)



  return a.exec();
}
