#include <QApplication>
#include "server/serverinterface.h"
#include "resourcemanager.h"
#include "scpi/scpiinterface.h"

int main(int argc, char* argv[])
{
  QCoreApplication a(argc,argv);

  //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // Protobuf does not like latin-1


  ResourceManager::getInstance();
  SCPI::SCPIInterface::getInstance();
  Server::ServerInterface::getInstance();

  return a.exec();
}
