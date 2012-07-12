#ifndef H2012_CLIENT_H
#define H2012_CLIENT_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>

namespace Resource
{
  class ResourceObject;
}

namespace Server
{
  /**
    @brief Server::Client represents an abstract client implementation, with a timeout/refresh function
    @todo This class needs unique identifiers for implementation clients.
    */
  class Client : public QObject
  {
    Q_OBJECT
  public:
    explicit Client(QString clientName, QObject *parent, QTcpSocket *clientSocket);
    /**
      @b returns the name of the client (something like RMS or Oscilloscope)
      */
    QString getName();
    /**
      @b returns the socket of the client
      */
    const QTcpSocket* getSocket();

  signals:
    /**
      @brief timeout raises when no comunication between server and client occurs within 5 seconds
      */
    void timeout();

  public slots:
    /**
      @brief Resets the timer back to 5 seconds.
      */
    void refresh();

    /**
      @brief Will be called if locking the resource caused errors
      */
    void lockFailed(Resource::ResourceObject *res);

  private:
    /**
      @b if the timer is called the client has a timeout
      It will be refreshed by the client in a set interval
      */
    QTimer timeoutCheck;

    /**
      @b name of the client (something like RMS or Oscilloscope)
      */
    const QString name;

    /**
      @b socket of the client connection
      */
    const QTcpSocket* socket;
  };
}

#endif // H2012_CLIENT_H
