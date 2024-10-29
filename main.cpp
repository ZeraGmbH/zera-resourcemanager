#include "resmanrunfacade.h"
#include <tcpnetworkfactory.h>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc,argv);
    ResmanRunFacade runner(VeinTcp::TcpNetworkFactory::create());
    return a.exec();
}
