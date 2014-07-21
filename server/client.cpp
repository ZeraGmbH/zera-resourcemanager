#include "server/client.h"

#include "server/clientmultiton.h"

#include <protonetpeer.h>
#include <QByteArray>

#include <QDebug>

namespace Server
{
  Client::Client(ProtoNetPeer *zClient, QObject *parent) :
    QObject(parent)
  {
    m_zClient = zClient;
    m_zClient->setParent(this);
    connect(m_zClient, &ProtoNetPeer::sigMessageReceived, this, &Client::onMessageReceived);
    connect(m_zClient, &ProtoNetPeer::sigConnectionClosed,this, &Client::sigAboutToDisconnect);
    connect(this, &Client::sigAboutToDisconnect,this, &Client::onDisconnectCleanup);
  }
  QString Client::getIpAdress()
  {
    return m_zClient->getIpAddress();
  }

  void Client::doSendACK(const QString &message, const QByteArray &cID)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ACK);
    newMessage->set_body(message.toStdString());

    if(!cID.isEmpty())
    {
      envelope.set_clientid(cID.data(),cID.size());
    }

    sendMessage(&envelope);
  }

  void Client::doSendDebug(const QString &message, const QByteArray &cID)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::DEBUG);
    newMessage->set_body(message.toStdString());

    if(!cID.isEmpty())
    {
      envelope.set_clientid(cID.data(),cID.size());
    }

    sendMessage(&envelope);
  }

  void Client::doSendError(const QString &message, const QByteArray &cID)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ERROR);
    newMessage->set_body(message.toStdString());

    if(!cID.isEmpty())
    {
      envelope.set_clientid(cID.data(),cID.size());
    }

    sendMessage(&envelope);
  }

  void Client::doSendNACK(const QString &message, const QByteArray &cID)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::NACK);
    newMessage->set_body(message.toStdString());

    if(!cID.isEmpty())
    {
      envelope.set_clientid(cID.data(),cID.size());
    }

    sendMessage(&envelope);
  }

  void Client::onMessageReceived(google::protobuf::Message *message)
  {
    ProtobufMessage::NetMessage *envelope = static_cast<ProtobufMessage::NetMessage *>(message);
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
          if(!m_clients.contains(baTemp))
          {
            ClientMultiton *tmpClientMult = new ClientMultiton(this, baTemp);
            m_clients.insert(baTemp,tmpClientMult);
          }
        }
      }
      if(m_clients.contains(baTemp))
      {
        m_clients.value(baTemp)->onMessageReceived(envelope);
      }
    }
    else
    {
      //legacy mode
      if(!m_clients.contains(QByteArray()))
      {
        ClientMultiton *emtpyBA = new ClientMultiton(this, QByteArray());
        m_clients.insert(QByteArray(),emtpyBA);
      }
      m_clients.value(QByteArray())->onMessageReceived(envelope);
    }
    if(envelope->has_netcommand())
    {
      if(m_clients.contains(baTemp))
      {
        delete m_clients.value(baTemp);
        m_clients.remove(baTemp);
      }
    }
    m_messageIdQueue.removeLast();
  }

  void Client::onDisconnectCleanup()
  {
    /// @todo TBD
    foreach (ClientMultiton *tmpClientM, m_clients.values()) {
      tmpClientM->onDisconnectCleanup();
      m_clients.remove(tmpClientM->getClientID());
      delete tmpClientM;
    }
  }

  void Client::sendMessage(ProtobufMessage::NetMessage *message)
  {
    qint64 tmp_mID = m_messageIdQueue.last();
    if(tmp_mID>0 && tmp_mID<4294967296) // check for legacy mode, the value has to fit into a uint32
    {
      message->set_messagenr(tmp_mID);
    }
    m_zClient->sendMessage(message);
  }
}
