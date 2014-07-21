#include "clientmultiton.h"

#include "server/client.h"
#include "scpi/scpiinterface.h"
#include <netmessages.pb.h>

#include <QDebug>

namespace Server
{
  ClientMultiton::ClientMultiton(Server::Client *parent, const QByteArray& cID) :
    QObject(parent), m_parent(parent), m_clientId(cID)
  {
  }

  QString ClientMultiton::name() const
  {
    return m_name;
  }

  void ClientMultiton::addOccupation(Application::Resource *res)
  {
    m_occupies.insert(res);
  }

  void ClientMultiton::removeOccupation(Application::Resource *res)
  {
    m_occupies.remove(res);
  }

  QString ClientMultiton::getIpAddress()
  {
    return m_parent->getIpAdress();
  }

  const QByteArray &ClientMultiton::getClientID()
  {
    return m_clientId;
  }

  void ClientMultiton::doSendACK(const QString &message)
  {
    m_parent->doSendACK(message, m_clientId);
  }

  void ClientMultiton::doSendDebug(const QString &message)
  {
    m_parent->doSendDebug(message, m_clientId);
  }

  void ClientMultiton::doSendError(const QString &message)
  {
    m_parent->doSendError(message, m_clientId);
  }

  void ClientMultiton::doSendNACK(const QString &message)
  {
    m_parent->doSendNACK(message, m_clientId);
  }

  void Server::ClientMultiton::onMessageReceived(ProtobufMessage::NetMessage *envelope)
  {
    //    ProtobufMessage::NetMessage *envelope = static_cast<ProtobufMessage::NetMessage *>(message);
    if(envelope->has_reply())
    {
      switch(envelope->reply().rtype())
      {
        case ProtobufMessage::NetMessage::NetReply::IDENT:
        {
          connect(this, &ClientMultiton::sigScpiTransaction, SCPI::SCPIInterface::getInstance(), &SCPI::SCPIInterface::onScpiTransaction);
          m_name = QString::fromStdString(envelope->reply().body());
          qDebug() << "Resourcemanager: Client identified" << m_name;
          doSendACK();
          break;
        }
        case ProtobufMessage::NetMessage::NetReply::ACK:
        {
          break;
        }
        case ProtobufMessage::NetMessage::NetReply::DEBUG:
        {
          qDebug() << QString("Client '%1' sent debug message:\n%2").arg(m_name).arg(QString::fromStdString(envelope->reply().body()));
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
      emit sigScpiTransaction(envelope->scpi());
    }
  }

  void ClientMultiton::onDisconnectCleanup()
  {
    foreach(Application::Resource *tmpRes, m_occupies)
    {
      tmpRes->freeResource(this);
    }
    SCPI::SCPIInterface::getInstance()->doResourceRemoveByProvider(this);
  }
}
