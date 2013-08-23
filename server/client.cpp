#include "server/client.h"

#include "QDebug"

#include <netmessages.pb.h>

namespace Server
{
  Client::Client(Zera::Net::cClient *zClient, QObject *parent) :
    QObject(parent)
  {
    m_zClient = zClient;
    m_zClient->setParent(this);
    connect(m_zClient,SIGNAL(messageReceived(QByteArray)),this,SLOT(messageReceived(QByteArray)));
    connect(m_zClient,SIGNAL(clientDisconnected()),this, SIGNAL(aboutToDisconnect()));
  }

  const QString &Client::getName()
  {
    return m_zClient->getName();
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

  void Client::messageReceived(QByteArray message)
  {
    // return message to client to show that it was received
    ProtobufMessage::NetMessage envelope;
    if(m_zClient->translateBA2Protobuf(&envelope, message))
    {
      if(envelope.has_reply())
      {
        switch(envelope.reply().rtype())
        {
          case ProtobufMessage::NetMessage::NetReply::IDENT:
          {
            m_zClient->setName(QString::fromStdString(envelope.reply().body()));
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
            qDebug() << QString("Client '%1' sent debug message:\n%2").arg(this->getName()).arg(QString::fromStdString(envelope.reply().body()));
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
      if(envelope.has_scpi())
      {
        emit scpiCommandSent(envelope.scpi());
      }
    }
    else
    {
      sendNACK(tr("Protocol error"));
      qDebug()<<"Resourcemanager: Error parsing protobuf";
    }
  }

  void Client::sendMessage(ProtobufMessage::NetMessage *message)
  {
    QByteArray block = m_zClient->translatePB2ByteArray(message);
    m_zClient->writeClient(block);
  }
}
