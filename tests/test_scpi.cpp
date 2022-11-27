#include "test_scpi.h"
#include "resourcemanager.h"
#include "resman_scpiinterface.h"
#include "clientmultitontest.h"
#include <QTest>

QTEST_MAIN(test_scpi)

void test_scpi::addValidResource()
{
    ResourceManager resMan;
    SCPI::SCPIInterface scpiInterface(&resMan);
    Application::ResourceIdentity::setSCPIInterface(&scpiInterface);
    ClientMultitonTest client("fooName", "fooIp");

    ProtobufMessage::NetMessage_ScpiCommand scpiCmd;
    scpiCmd.set_command("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");

    scpiInterface.onScpiTransaction(&client, scpiCmd);
    QCOMPARE(client.getAckList().count(), 1);
}

void test_scpi::addInvalidParamCount()
{
    ResourceManager resMan;
    SCPI::SCPIInterface scpiInterface(&resMan);
    Application::ResourceIdentity::setSCPIInterface(&scpiInterface);
    ClientMultitonTest client("fooName", "fooIp");

    ProtobufMessage::NetMessage_ScpiCommand scpiCmd;
    scpiCmd.set_command("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;");

    scpiInterface.onScpiTransaction(&client, scpiCmd);
    QCOMPARE(client.getNackList().count(), 1);
}

void test_scpi::addInvalidPort()
{
    ResourceManager resMan;
    SCPI::SCPIInterface scpiInterface(&resMan);
    Application::ResourceIdentity::setSCPIInterface(&scpiInterface);
    ClientMultitonTest client("fooName", "fooIp");

    ProtobufMessage::NetMessage_ScpiCommand scpiCmd;
    scpiCmd.set_command("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;PORT;");

    scpiInterface.onScpiTransaction(&client, scpiCmd);
    QCOMPARE(client.getNackList().count(), 1);
}

void test_scpi::invalidScpi()
{
    ResourceManager resMan;
    SCPI::SCPIInterface scpiInterface(&resMan);
    Application::ResourceIdentity::setSCPIInterface(&scpiInterface);
    ClientMultitonTest client("fooName", "fooIp");

    ProtobufMessage::NetMessage_ScpiCommand scpiCmd;
    scpiCmd.set_command("RESO:ADD SENSE;FOONAME;1;FOODESC;PORT;");

    scpiInterface.onScpiTransaction(&client, scpiCmd);
    QCOMPARE(client.getErrList().count(), 1);
}
