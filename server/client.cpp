#include "server/client.h"

namespace Server
{
  Client::Client(int socketDescriptor, QObject *parent) :
    QObject(parent)
  {
    name="";
    sockDescriptor=socketDescriptor;
    connect(&timeoutCheck, SIGNAL(timeout()), this,SIGNAL(timeout()));

    if(!clSocket.setSocketDescriptor(sockDescriptor))
    {
      emit error(clSocket.error());
      qDebug()<<"error setting clients socket descriptor";
      return;
    }
  }



  void Client::run()
  {
    /* */
    while(internalState!=States::disconnected)
    {
      switch(internalState)
      {
        case States::notConnected:
          {
            /** start the timer with 150 mseconds timeout */
            timeoutCheck.start(150);
            initConnection();
            break;
          }
        case States::connected:
          {
            maintainConnection();
            break;
          }
        case States::aboutToDisconnect:
          {
            closeConnection();
            break;
          }
        case States::disconnected:
        default:
          {
            //error
            break;
          }
      }
    }
    /* */

    //debug
    QTcpSocket clSocket;
    if(!clSocket.setSocketDescriptor(sockDescriptor))
    {
      emit error(clSocket.error());
      qDebug()<<"error";
      return;
    }

    char buffer[1024] = {0};
    clSocket.read(buffer, clSocket.bytesAvailable());
    qDebug() <<  "Client: " << buffer << endl;

    QString text="ACK";
    quint8 number=1;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << text;
    out << number;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    clSocket.write(block);
    clSocket.waitForBytesWritten();
    clSocket.close();

    emit clientFinished();
  }

  void Client::closeConnection()
  {
  }

  void Client::initConnection()
  {
    writeClient(HCStrings::PreformattedMessages.at(HCStrings::welcome));
    name=readClient();

    if(name!="")
    {
      transitToState(States::connected);
    }

  }

  void Client::maintainConnection()
  {
    QString command=readClient();
    if(command==HCStrings::PreformattedMessages.at(HCStrings::refresh))
    {
      refresh();
    }
    else
    {
      refresh();//refresh anyway, the other check is only to filter out pure refreshes
      emit scpiCommandSent(command);
    }
  }

  void Client::transitToState(States::ClientStates newState)
  {
    if(internalState!=newState)
    {
      //The client has connected
      if(internalState==States::notConnected&&newState==States::connected)
      {
        internalState=newState;
      }
      //The client will disconnect soon
      if(internalState==States::connected&&newState==States::aboutToDisconnect)
      {
        internalState=newState;
      }
      //The client is gone and this instance will be deleted in a few cycles
      if(internalState==States::aboutToDisconnect&&newState==States::disconnected)
      {
        emit clientFinished();
        internalState=newState;
      }
    }
  }

  QString Client::readClient()
  {
    if(clSocket.waitForReadyRead(150))//wait for 150 mseconds max
    {
      refresh(); //the connection is alive
      QString retVal;
      QDataStream in(&clSocket);
      in.setVersion(QDataStream::Qt_4_0);
      quint16 tmp;
      in >> tmp;
      if(clSocket.bytesAvailable()<tmp)
      {
        qDebug()<<"Error bytes not available";
        retVal="";
      }
      else
      {
        in >> retVal;
      }      
      return retVal;
    }
    else
      return "";
  }

  void Client::writeClient(QString message)
  {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    clSocket.write(block);
    if(clSocket.waitForBytesWritten(150)) // wait 150 mseconds max
      refresh(); //the connection is alive

  }

  QString Client::getName()
  {
    return name;
  }

  int Client::getSocket()
  {
    return sockDescriptor;
  }

  /// @todo REMOVE!
  void Client::test(QString message)
  {
    scpiCommandSent(message);
  }

  void Client::refresh()
  {
    timeoutCheck.start(150);
  }

  void Client::sendToClient(QString message)
  {
    clMutex.lock();
    writeClient(message);
    if(readClient()!=HCStrings::PreformattedMessages.at(HCStrings::accepted))
    {
      //error
    }
    clMutex.unlock();
  }
}
