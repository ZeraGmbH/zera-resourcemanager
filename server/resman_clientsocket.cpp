#include "resman_clientsocket.h"

#include "resman_clientmultiton.h"

#include <xiqnetpeer.h>
#include <QByteArray>

#include <QDebug>

namespace ResourceServer
{
ClientSocket::ClientSocket(XiQNetPeer *t_clientSocket, QObject *t_parent) :
    QObject(t_parent),
    m_zClient(t_clientSocket)
{
    Q_ASSERT(t_clientSocket != nullptr);

    m_zClient->setParent(this);
    connect(m_zClient, &XiQNetPeer::sigMessageReceived, this, &ClientSocket::onMessageReceived);
    connect(m_zClient, &XiQNetPeer::sigConnectionClosed, this, &ClientSocket::sigAboutToDisconnect);
    connect(this, &ClientSocket::sigAboutToDisconnect, this, &ClientSocket::onDisconnectCleanup);
}

ClientSocket::~ClientSocket()
{
    delete m_zClient;
}

QString ClientSocket::getIpAdress() const
{
    return m_zClient->getIpAddress();
}

void ClientSocket::doSendACK(const QString &t_message, const QByteArray &t_cID) const
{
    ProtobufMessage::NetMessage envelope = ProtobufMessage::NetMessage();
    ProtobufMessage::NetMessage::NetReply *newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ACK);
    newMessage->set_body(t_message.toStdString());

    if(!t_cID.isEmpty())
    {
        envelope.set_clientid(t_cID.data(),t_cID.size());
    }

    sendMessage(envelope);
}

void ClientSocket::doSendDebug(const QString &t_message, const QByteArray &t_cID) const
{
    ProtobufMessage::NetMessage envelope = ProtobufMessage::NetMessage();
    ProtobufMessage::NetMessage::NetReply *newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::DEBUG);
    newMessage->set_body(t_message.toStdString());

    if(!t_cID.isEmpty())
    {
        envelope.set_clientid(t_cID.data(),t_cID.size());
    }

    sendMessage(envelope);
}

void ClientSocket::doSendError(const QString &t_message, const QByteArray &t_cID) const
{
    ProtobufMessage::NetMessage envelope = ProtobufMessage::NetMessage();
    ProtobufMessage::NetMessage::NetReply *newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ERROR);
    newMessage->set_body(t_message.toStdString());

    if(!t_cID.isEmpty())
    {
        envelope.set_clientid(t_cID.data(),t_cID.size());
    }

    sendMessage(envelope);
}

void ClientSocket::doSendNACK(const QString &t_message, const QByteArray &t_cID) const
{
    ProtobufMessage::NetMessage envelope = ProtobufMessage::NetMessage();
    ProtobufMessage::NetMessage::NetReply *newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::NACK);
    newMessage->set_body(t_message.toStdString());

    if(!t_cID.isEmpty())
    {
        envelope.set_clientid(t_cID.data(),t_cID.size());
    }

    sendMessage(envelope);
}

void ClientSocket::onMessageReceived(std::shared_ptr<google::protobuf::Message> t_message)
{
    std::shared_ptr<ProtobufMessage::NetMessage> envelope = nullptr;
    envelope = std::static_pointer_cast<ProtobufMessage::NetMessage>(t_message);
    Q_ASSERT(envelope != nullptr);

    // return message to client to show that it was received
    QByteArray baTemp;
    if(envelope->has_messagenr())
    {
        m_messageIdQueue.enqueue(envelope->messagenr());
    }
    else
    {
        //legacy mode
        m_messageIdQueue.enqueue(-1);
    }
    if(envelope->has_clientid())
    {
        baTemp = QByteArray(envelope->clientid().data(),envelope->clientid().size());
        if(envelope->has_reply())
        {
            if(envelope->reply().rtype() == ProtobufMessage::NetMessage::NetReply::IDENT)
            {
                if(!m_clientSockets.contains(baTemp))
                {
                    ClientMultiton *tmpClientMult = new ClientMultiton(this, baTemp);
                    m_clientSockets.insert(baTemp,tmpClientMult);
                    emit sigClientIdentified(tmpClientMult);
                }
            }
        }
        if(m_clientSockets.contains(baTemp))
        {
            m_clientSockets.value(baTemp)->onMessageReceived(envelope);
        }
    }
    else
    {
        //legacy mode
        if(!m_clientSockets.contains(QByteArray()))
        {
            ClientMultiton *emtpyBA = new ClientMultiton(this, QByteArray());
            m_clientSockets.insert(QByteArray(),emtpyBA);
            emit sigClientIdentified(emtpyBA);
        }
        m_clientSockets.value(QByteArray())->onMessageReceived(envelope);
    }
    if(envelope->has_netcommand())
    {
        if(envelope->netcommand().cmd() == ProtobufMessage::NetMessage::NetCmd::CmdType::NetMessage_NetCmd_CmdType_RELEASE
                && m_clientSockets.contains(baTemp))
        {
            delete m_clientSockets.value(baTemp);
            m_clientSockets.remove(baTemp);
        }
    }
    m_messageIdQueue.removeLast();
}

void ClientSocket::onDisconnectCleanup()
{
    const auto tmpClients = m_clientSockets.values();
    for(ClientMultiton *tmpClientM : qAsConst(tmpClients))
    {
        emit sigClientMultitonDisconnected(tmpClientM);
        delete tmpClientM;
    }
    m_clientSockets.clear();
}

void ClientSocket::sendMessage(ProtobufMessage::NetMessage &t_message) const
{
    qint64 tmp_mID = m_messageIdQueue.last();
    if(tmp_mID>0 && tmp_mID<4294967296) // check for legacy mode, the value has to fit into a uint32
    {
        t_message.set_messagenr(tmp_mID);
    }
    m_zClient->sendMessage(t_message);
}
}
