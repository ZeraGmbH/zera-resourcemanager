#include "test_scpi.h"
#include <QTest>

QTEST_MAIN(test_scpi)

void test_scpi::init()
{
    m_resMan = new ResourceManager;
    m_scpiInterface = new SCPI::SCPIInterface(m_resMan);
    Application::ResourceIdentity::setSCPIInterface(m_scpiInterface);
    m_client = new ClientMultitonTest("fooName", "fooIp");
}

void test_scpi::cleanup()
{
    delete m_client;
    delete m_scpiInterface;
    delete m_resMan;
}

void test_scpi::addValidResource()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    QCOMPARE(m_client->getAckList().count(), 1);
}

void test_scpi::addValidResourceAndCatalog()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    sendScpi("RES:SENSE:CAT?");

    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 2);
    QCOMPARE(ackList[1], "FOONAME");
}

void test_scpi::addValidResourceAndCatalogType()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    sendScpi("RESOURCE:ADD CALC;FOONAME;1;FOODESC;1000;");
    sendScpi("RES:TYP:CAT?");

    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 3);
    QStringList typeList = ackList[2].split(";");
    QCOMPARE(typeList.count(), 2);
    QVERIFY(typeList.contains("SENSE"));
    QVERIFY(typeList.contains("CALC"));
}

void test_scpi::addValidResourceTwice()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    QCOMPARE(m_client->getAckList().count(), 2);
}

void test_scpi::addValidResourceAndRemove()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    sendScpi("RESOURCE:REM SENSE;FOONAME;");
    sendScpi("RES:TYP:CAT?");

    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 3);
    QCOMPARE(ackList[2], "");
}

void test_scpi::addValidResourceAndQuery()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOONAME?");

    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 2);
    QStringList resList = ackList[1].split(";", Qt::SkipEmptyParts);
    QCOMPARE(resList.count(), 4);
    QCOMPARE(resList[0], "5");      // total amount
    QCOMPARE(resList[1], "5");      // free amount
    QCOMPARE(resList[2], "FOODESC");// description
    QCOMPARE(resList[3], "1000");   // port
}

void test_scpi::addNestedAndQuery()
{
    sendScpi("RESOURCE:ADD SENSE:FOO;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOO:FOONAME?");
    QCOMPARE(m_client->getErrList().count(), 1); // !!! inserted but not found
    sendScpi("RESOURCE:MODEL?");
    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 2);
    //qWarning("%s", qPrintable(ackList[1]));
}

void test_scpi::addNestedAsParamAndQuery()
{
    sendScpi("RESOURCE:ADD SENSE;FOO:FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOO:FOONAME?");
    QCOMPARE(m_client->getErrList().count(), 1); // !!! inserted but not found
    sendScpi("RESOURCE:MODEL?");
    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 2);
    //qWarning("%s", qPrintable(ackList[1]));
}

void test_scpi::addValidResourceAndSetAndQuery()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOONAME SET;1;");
    sendScpi("RESOURCE:SENSE:FOONAME?");

    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 3);
    QStringList resList = ackList[2].split(";", Qt::SkipEmptyParts);
    QCOMPARE(resList.count(), 4);
    QCOMPARE(resList[0], "5");      // total amount
    QCOMPARE(resList[1], "4");      // free amount
}

void test_scpi::addValidResourceAndSetAndFreeAndQuery()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOONAME SET;3;");
    sendScpi("RESOURCE:SENSE:FOONAME FREE;1;");
    sendScpi("RESOURCE:SENSE:FOONAME?");

    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 4);
    QStringList resList = ackList[3].split(";", Qt::SkipEmptyParts);
    QCOMPARE(resList.count(), 4);
    QCOMPARE(resList[0], "5");      // total amount
    QCOMPARE(resList[1], "5");      // free amount
}

void test_scpi::addInvalidParamCount()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;");
    QCOMPARE(m_client->getNackList().count(), 1);
}

void test_scpi::addValidAndRemoveInvalidSub()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;1000;");
    sendScpi("RESOURCE:REM SENSE;FOONAME;HONK;");
    sendScpi("RES:TYP:CAT?");

    QCOMPARE(m_client->getNackList().count(), 1);
    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 2);
}

void test_scpi::addInvalidPort()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;1;FOODESC;PORT;");
    QCOMPARE(m_client->getNackList().count(), 1);
}

void test_scpi::invalidScpi()
{
    sendScpi("RESO:ADD SENSE;FOONAME;1;FOODESC;PORT;");
    QCOMPARE(m_client->getErrList().count(), 1);
}

void test_scpi::freeOnEmpty()
{
    sendScpi("RESOURCE:REM SENSE;FOONAME;");
    QCOMPARE(m_client->getNackList().count(), 1);
}

void test_scpi::setMoreThanAvailable()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOONAME SET;6;");
    QCOMPARE(m_client->getAckList().count(), 1);
    QCOMPARE(m_client->getNackList().count(), 1);
}

void test_scpi::freeUnset()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOONAME FREE;1;");
    QCOMPARE(m_client->getAckList().count(), 1);
    QCOMPARE(m_client->getNackList().count(), 1);
}

void test_scpi::freeMoreThanSet()
{
    sendScpi("RESOURCE:ADD SENSE;FOONAME;5;FOODESC;1000;");
    sendScpi("RESOURCE:SENSE:FOONAME SET;1;");
    sendScpi("RESOURCE:SENSE:FOONAME FREE;2;");
    sendScpi("RESOURCE:SENSE:FOONAME?");

    // Expected (as in freeUnset):
    /*QCOMPARE(m_client->getNackList().count(), 1);
    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 3);
    QStringList resList = ackList[2].split(";", Qt::SkipEmptyParts);
    QCOMPARE(resList.count(), 4);
    QCOMPARE(resList[0], "5");      // total amount
    QCOMPARE(resList[1], "4");      // free amount*/

    // Found
    QStringList ackList = m_client->getAckList();
    QCOMPARE(ackList.count(), 4);
    QStringList resList = ackList[3].split(";", Qt::SkipEmptyParts);
    QCOMPARE(resList.count(), 4);
    QCOMPARE(resList[0], "5");      // total amount
    QCOMPARE(resList[1], "5");      // free amount
}

void test_scpi::sendScpi(const char* cmd)
{
    ProtobufMessage::NetMessage_ScpiCommand scpiCmd;
    scpiCmd.set_command(cmd);
    m_scpiInterface->onScpiTransaction(m_client, scpiCmd);
}
