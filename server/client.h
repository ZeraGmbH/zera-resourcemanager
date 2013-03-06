#ifndef H2012_CLIENT_H
#define H2012_CLIENT_H

#include <QStringList>
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QMutex>

namespace Application
{
  class Resource;
}

namespace Server
{

  namespace States
  {
    enum ClientStates
    {
      notConnected=0, /**< The default state */
      connected, /**< The client has connected */
      aboutToDisconnect, /**< The client will disconnect soon */
      disconnected /**< The client is gone and this instance will be deleted in a few cycles */
    };
  }

  /**
    @brief Contains hardcoded strings
    */
  namespace HCStrings
  {
    /**
      @b Default message ids
      @see Server::HCStrings::PreformattedMessages
      */
    enum MessageIDs
    {
      accepted=0,/**< The general "command was accepted" message */
      disconnect, /**< The client wants to disconnect */
      refresh, /**< The client will send this message to refresh the timeout counter */
      welcome /**< The client will recieve this message when he connects */
    };
    /**
      @b Default hardcoded messages, case insensitive
      */
    static QStringList PreformattedMessages=QStringList()<<"ack"
                                                        <<"disconnect"
                                                       <<"refresh"
                                                      <<"Connected to Resourcemanager server";
  }


  /**
    @brief Server::Client represents an abstract client implementation, with a timeout/refresh function
    @todo This class needs unique identifiers for implementation clients.
    */
  class Client : public QObject
  {
    Q_OBJECT
  public:
    /**
      @b The default constructor
      @note Other constructors are invalid
      */
    explicit Client(int socketDescriptor, QObject *parent = 0);
    /**
      @b returns the name of the client (something like RMS or Oscilloscope)
      */
    const QString &getName();
    /**
      @b returns the socket descriptor of the clients socket
      */
    int getSocket();

    void testScpiCommand(const QString &message);

  signals:
    /**
      @b Notifies the SCPI::SCPIInterface of new SCPI commands
      */
    void scpiCommandSent(const QString &command);

    /**
      @b Socket error fallback
      */
    void error(QTcpSocket::SocketError socketError);
    /**
      @brief timeout raises when no communication between server and client occurs within 5 seconds
      */
    void timeout();

    /**
      @b The client finished all transactions and can be disconnected
      */
    void clientFinished();

  private:
    /**
      @b Disconnects the client
      */
    void closeConnection();
    /**
      @b Creates a connection to the client
      */
    void initConnection();
    /**
      @b Wait for commands
      */
    void maintainConnection();
    /**
      @b State machine logic checker
      */
    void transitToState(States::ClientStates newState);

    /**
      @b Reads a QString from the socket
      */
    const QString readClient();
    /**
      @b Writes a QString to the socket
      */
    void writeClient(const QString &message);

    /**
      @b The current state in the state machine
      */
    States::ClientStates internalState;

    /**
      @b will be used to write messages back to the client
      */
    QMutex clMutex;

    /**
      @b The actual socket of the Server::Client, this object cannot be used cross thread wise
      */
    QTcpSocket clSocket;
    /**
      @b if the timer is called the client has a timeout
      It will be refreshed by the client in a set interval
      */
    QTimer timeoutCheck;

    /**
      @b name of the client (something like RMS or Oscilloscope)
      */
    QString name;

    /**
      @b socket of the client connection
      */
    int sockDescriptor;

  public slots:
    /**
      @brief Will be called if locking the resource caused errors
      */
    void sendToClient(const QString &message);

    /**
      @brief Resets the timer back to 5 seconds.
      */
    void refresh();

    void run();

  };
}

#endif // H2012_CLIENT_H
