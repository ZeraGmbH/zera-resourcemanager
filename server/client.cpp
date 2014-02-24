#include "server/client.h"

#include "QDebug"
#include <protonetpeer.h>
#include <netmessages.pb.h>

namespace Server
{
  Client::Client(ProtoNetPeer *zClient, QObject *parent) :
    QObject(parent)
  {
    m_zClient = zClient;
    m_zClient->setParent(this);
    connect(m_zClient,SIGNAL(sigMessageReceived(google::protobuf::Message*)), this, SLOT(messageReceived(google::protobuf::Message*)));
    connect(m_zClient,SIGNAL(sigConnectionClosed()),this, SIGNAL(aboutToDisconnect()));
  }

  const QString &Client::getName()
  {
    return m_name;
  }

  QString Client::getIpAdress()
  {
    return m_zClient->getIpAddress().toString();
  }

  void Client::sendACK(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ACK);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::sendDebug(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::DEBUG);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::sendError(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::ERROR);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::sendNACK(const QString &message)
  {
    ProtobufMessage::NetMessage envelope;
    ProtobufMessage::NetMessage::NetReply* newMessage = envelope.mutable_reply();
    newMessage->set_rtype(ProtobufMessage::NetMessage::NetReply::NACK);
    newMessage->set_body(message.toStdString());

    sendMessage(&envelope);
  }

  void Client::messageReceived(google::protobuf::Message *message)
  {
    ProtobufMessage::NetMessage *envelope = static_cast<ProtobufMessage::NetMessage *>(message);
    // return message to client to show that it was received
    if(envelope->has_clientid())
    {
      clientIdQueue.enqueue(envelope->clientid());
    }
    if(envelope->has_messagenr())
    {
      messageIdQueue.enqueue(envelope->messagenr());
    }
    else
    {
      //legacy mode
      clientIdQueue.enqueue("");
      messageIdQueue.enqueue(-1);
    }
    if(envelope->has_reply())
    {
      switch(envelope->reply().rtype())
      {
        case ProtobufMessage::NetMessage::NetReply::IDENT:
        {
          m_name = QString::fromStdString(envelope->reply().body());
          qDebug() << "Resourcemanager: Client identified" << getName();
          sendACK();
          break;
        }
        case ProtobufMessage::NetMessage::NetReply::ACK:
        {
          /// @todo are we expecting a reply from a client or is this a defect?
          break;
        }
        case ProtobufMessage::NetMessage::NetReply::DEBUG:
        {
          //no reply is being sent
          clientIdQueue.removeLast();
          messageIdQueue.removeLast();
          qDebug() << QString("Client '%1' sent debug message:\n%2").arg(this->getName()).arg(QString::fromStdString(envelope->reply().body()));
          break;
        }
        default:
        {
          //no reply is being sent
          clientIdQueue.removeLast();
          messageIdQueue.removeLast();
          qWarning("Resourcemanager: Something went wrong with network messages!");
          /// @todo this is the error case
          break;
        }
      }
    }
    if(envelope->has_scpi())
    {
      emit scpiCommandSent(envelope->scpi());
    }
  }

  void Client::sendMessage(ProtobufMessage::NetMessage *message)
  {
    std::string tmp_cID = clientIdQueue.dequeue();
    qint64 tmp_mID = messageIdQueue.dequeue();
    if(tmp_cID!="") //check for legacy mode
    {
      message->set_clientid(tmp_cID);
    }
    if(tmp_mID>0 && tmp_mID<4294967296) // check for legacy mode, the value has to fit into a uint32
    {
      message->set_messagenr(tmp_mID);
    }
    m_zClient->sendMessage(message);
  }
}
