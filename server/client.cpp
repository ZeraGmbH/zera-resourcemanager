#include "server/client.h"

#include "QDebug"
#include <protonetpeer.h>
#include <netmessages.pb.h>

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

  const QString &Client::getName()
  {
    return m_name;
  }

  QString Client::getIpAdress()
  {
    return m_zClient->getIpAddress();
  }

  void Client::addOccupation(Application::Resource *res)
  {
    m_occupies.insert(res);
  }

  void Client::removeOccupation(Application::Resource *res)
  {
    m_occupies.remove(res);
  }

  void Client::doSendACK(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ACK);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::doSendDebug(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::DEBUG);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::doSendError(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ERROR);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::doSendNACK(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::NACK);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::onMessageReceived(google::protobuf::Message *message)
  {
    ProtobufMessage::NetMessage *envelope = static_cast<ProtobufMessage::NetMessage *>(message);
    // return message to client to show that it was received
    if(envelope->has_clientid())
    {
      //qDebug() << "queued clientid:"<< QByteArray(envelope->clientid().c_str(), envelope->clientid().length()).toBase64();
      m_clientIdQueue.enqueue(QByteArray(envelope->clientid().data(),envelope->clientid().size()));
    }
    else
    {
      //legacy mode
      m_clientIdQueue.enqueue(QByteArray());
    }
    if(envelope->has_messagenr())
    {
      m_messageIdQueue.enqueue(envelope->messagenr());
    }
    else
    {
      //legacy mode
      m_messageIdQueue.enqueue(-1);
    }
    if(envelope->has_netcommand())
    {
      //qDebug() << "## clientId:" << m_clientIdQueue;
      foreach (Application::Resource *tmpRes, m_occupies) {
        if(tmpRes->getProviderId()==m_clientIdQueue.last())
        {
          tmpRes->freeResource(this);
        }
      }
      emit sigDisconnectedClientId(m_clientIdQueue.last());
    }
    if(envelope->has_reply())
    {
      switch(envelope->reply().rtype())
      {
        case ProtobufMessage::NetMessage::NetReply::IDENT:
        {
          m_name = QString::fromStdString(envelope->reply().body());
          qDebug() << "Resourcemanager: Client identified" << getName();
          doSendACK();
          break;
        }
        case ProtobufMessage::NetMessage::NetReply::ACK:
        {
          break;
        }
        case ProtobufMessage::NetMessage::NetReply::DEBUG:
        {
          qDebug() << QString("Client '%1' sent debug message:\n%2").arg(this->getName()).arg(QString::fromStdString(envelope->reply().body()));
          break;
        }
        default:
        {
          qWarning("Resourcemanager: Something went wrong with network messages!");
          /// @todo this is the error case
          break;
        }
      }
    }
    if(envelope->has_scpi())
    {
      emit sigScpiTransaction(envelope->scpi(),m_clientIdQueue.last());
    }
    m_clientIdQueue.removeLast();
    m_messageIdQueue.removeLast();
  }

  void Client::onDisconnectCleanup()
  {
    foreach (Application::Resource *tmpRes, m_occupies) {
      tmpRes->freeResource(this);
    }
  }

  void Client::sendMessage(ProtobufMessage::NetMessage *message)
  {
    QByteArray tmp_cID = m_clientIdQueue.last();
    qint64 tmp_mID = m_messageIdQueue.last();
    if(!tmp_cID.isEmpty()) //check for legacy mode
    {
      //qDebug()  << "clientid sent:" << tmp_cID.toBase64();
      message->set_clientid(tmp_cID.data(),tmp_cID.size());
    }
    if(tmp_mID>0 && tmp_mID<4294967296) // check for legacy mode, the value has to fit into a uint32
    {
      message->set_messagenr(tmp_mID);
    }
    m_zClient->sendMessage(message);
  }
}
