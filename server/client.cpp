#include "server/client.h"

namespace Server
{
  Client::Client(int socketDescriptor, QObject *parent) :
    QObject(parent)
  {
    internalState=States::notConnected;
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

    emit clientFinished();
  }

  void Client::closeConnection()
  {
    clSocket.close();
    transitToState(States::disconnected);
  }

  void Client::initConnection()
  {
    if(!clSocket.setSocketDescriptor(sockDescriptor))
    {
      emit error(clSocket.error());
      qDebug()<<"error";
      return;
    }

    writeClient(HCStrings::PreformattedMessages.at(HCStrings::welcome));
    name=readClient();

    if(name!="")
    {
      transitToState(States::connected);
    }
    else
    {
      /// @TODO
      // error?
    }
  }

  void Client::maintainConnection()
  {
    QString command=readClient();
    if(command==HCStrings::PreformattedMessages.at(HCStrings::refresh))
    {
      refresh();
    }
    else if(command==HCStrings::PreformattedMessages.at(HCStrings::disconnect))
    {
      /// @TODO
    }
    else /*if(isScpiCommand(command)) */ /// @TODO insert check if the command is a valid SCPI command
    {
      refresh();//refresh anyway, the other check is only to filter out pure refreshes
      emit scpiCommandSent(command);
    }
    /*
    else
    {
      // error
    }
    */
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

  const QString Client::readClient()
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

  void Client::writeClient(const QString &message)
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

  const QString &Client::getName()
  {
    return name;
  }

  int Client::getSocket()
  {
    return sockDescriptor;
  }

  /// @todo REMOVE!
  void Client::testScpiCommand(const QString &message)
  {
    scpiCommandSent(message);
  }

  void Client::refresh()
  {
    timeoutCheck.start(150);
  }

  void Client::sendToClient(const QString &message)
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
