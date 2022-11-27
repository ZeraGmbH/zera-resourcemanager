#include "clientmultitontest.h"

ClientMultitonTest::ClientMultitonTest(QString name, QString ipAddress) :
    m_name(name),
    m_ipAddress(ipAddress)
{
}

QString ClientMultitonTest::getName() const
{
    return m_name;
}

QString ClientMultitonTest::getIpAddress() const
{
    return m_ipAddress;
}

void ClientMultitonTest::doSendACK(const QString &t_message) const
{
    m_ackList.append(t_message);
}

void ClientMultitonTest::doSendError(const QString &t_message) const
{
    m_errList.append(t_message);
}

void ClientMultitonTest::doSendNACK(const QString &t_message) const
{
    m_nackList.append(t_message);
}

QStringList ClientMultitonTest::getAckList() const
{
    return m_ackList;
}

QStringList ClientMultitonTest::getNackList() const
{
    return m_nackList;
}

QStringList ClientMultitonTest::getErrList() const
{
    return m_errList;
}

void ClientMultitonTest::onMessageReceived(std::shared_ptr<ProtobufMessage::NetMessage> t_envelope)
{

}
