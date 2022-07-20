#include "resmanrunfacade.h"
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc,argv);

    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // Protobuf does not like latin-1

    ResmanRunFacade runner;
    return a.exec();
}
