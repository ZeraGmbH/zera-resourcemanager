#include <QCoreApplication>
#include "server/resman_serverinterface.h"
#include "resourcemanager.h"
#include "scpi/resman_scpiinterface.h"
#include "resource/resman_resource.h"
#include "resource/resman_resourceidentity.h"

int main(int argc, char* argv[])
{
  QCoreApplication a(argc,argv);

  //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // Protobuf does not like latin-1


  ResourceManager resMan;
  SCPI::SCPIInterface scpiInterface(&resMan);
  Application::ResourceIdentity::setSCPIInterface(&scpiInterface);
  ResourceServer::ServerInterface serverInterface(&scpiInterface);

  return a.exec();
}
