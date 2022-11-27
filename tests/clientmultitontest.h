#ifndef CLIENTMULTITONTEST_H
#define CLIENTMULTITONTEST_H

#include "resman_iclientmultiton.h"
#include <QStringList>

/**
 * @brief ClientMultitonTest: Mock for resman client collecting responses on
 * SCPI commands
 */
class ClientMultitonTest : public ResourceServer::IClientMultiton
{
    Q_OBJECT
public:
    ClientMultitonTest(QString name, QString ipAddress);
    QString getName() const override;
    QString getIpAddress() const override;
public:
    void doSendACK(const QString &t_message = QString()) const override;
    void doSendError(const QString &t_message = QString()) const override;
    void doSendNACK(const QString &t_message = QString()) const override;
    QStringList getAckList() const;
    QStringList getNackList() const;
    QStringList getErrList() const;
public slots:
    void onMessageReceived(std::shared_ptr<ProtobufMessage::NetMessage> t_envelope) override;

private:
    QString m_name;
    QString m_ipAddress;
    mutable QStringList m_ackList;
    mutable QStringList m_nackList;
    mutable QStringList m_errList;
};

#endif // CLIENTMULTITONTEST_H
