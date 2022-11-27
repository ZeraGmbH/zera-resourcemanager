#ifndef RESMAN_ICLIENTMULTITON_H
#define RESMAN_ICLIENTMULTITON_H

#include <QObject>
#include <QString>
#include <netmessages.pb.h>

namespace ResourceServer
{
class IClientMultiton : public QObject
{
    Q_OBJECT
public:
    virtual QString getName() const = 0;
    virtual QString getIpAddress() const = 0;
signals:
    /**
      *@brief Notifies the SCPI::SCPIInterface of new SCPI commands
      */
    void sigScpiTransaction(IClientMultiton *t_clientMultiton, const ProtobufMessage::NetMessage_ScpiCommand &t_command);
public:
    /**
    * @brief Sends acknowledgement
    * @param message Optional text
    */
    virtual void doSendACK(const QString &t_message = QString()) const = 0;
    virtual void doSendError(const QString &t_message = QString()) const = 0;
    virtual void doSendNACK(const QString &t_message = QString()) const = 0;
public slots:
    virtual void onMessageReceived(std::shared_ptr<ProtobufMessage::NetMessage> t_envelope) = 0;
};
}

#endif // RESMAN_ICLIENTMULTITON_H
