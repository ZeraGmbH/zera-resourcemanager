#include "resmanrunfacade.h"

ResmanRunFacade::ResmanRunFacade() :
    m_scpiInterface(&m_resMan),
    m_serverInterface(&m_scpiInterface)
{
    Application::ResourceIdentity::setSCPIInterface(&m_scpiInterface);
    m_serverInterface.start();
}
