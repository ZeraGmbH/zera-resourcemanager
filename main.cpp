#include "resmanrunfacade.h"
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc,argv);
    ResmanRunFacade runner;
    return a.exec();
}
