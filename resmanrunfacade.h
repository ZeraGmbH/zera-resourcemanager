#ifndef RESMANRUNFACADE_H
#define RESMANRUNFACADE_H

#include "resman_serverinterface.h"
#include "resourcemanager.h"
#include "resman_scpiinterface.h"
#include "resman_resource.h"
#include "resman_resourceidentity.h"

class ResmanRunFacade
{
public:
    ResmanRunFacade();
private:
    ResourceManager m_resMan;
    SCPI::SCPIInterface m_scpiInterface;
    ResourceServer::ServerInterface m_serverInterface;
};

#endif // RESMANRUNFACADE_H
