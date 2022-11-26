#include "test_legacy.h"
#include "resourcemanager.h"
#include "resman_scpiinterface.h"
#include <QTest>

QTEST_MAIN(test_legacy)

void test_legacy::initialCatalog()
{
    ResourceManager resMan;
    SCPI::SCPIInterface scpiInterface(&resMan);
    Application::ResourceIdentity::setSCPIInterface(&scpiInterface);
}
