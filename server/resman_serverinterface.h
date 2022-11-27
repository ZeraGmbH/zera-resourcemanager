#ifndef H2012_SERVERINTERFACE_H
#define H2012_SERVERINTERFACE_H


#include <QObject>
#include <QSet>

class XiQNetServer;
class XiQNetPeer;

class RMProtobufWrapper;

namespace Application
{
class Resource;
}

namespace SCPI
{
class SCPIInterface;
}

/**
  @brief The Server Namespace encapsulates all networking logic into a separate namespace.
 */
namespace ResourceServer
{
class IClientMultiton;
class ClientSocket;
/**
  @brief Represents the interface between the network implementations and the ResourceManager
  */
class ServerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ServerInterface(SCPI::SCPIInterface *t_scpiInterface, QObject *t_parent = 0);
    ~ServerInterface();
    void start();

public slots:
    /**
      @brief Will be triggered when the Server::Client disconnects
      */
    void clientDisconnected(ClientSocket *t_clientSocket);

    /**
     * @brief Is called when new clients arrive
     * @param [in] t_newClient the Zera::Net::cClient that just connected
     */
    void newClient(XiQNetPeer *t_newClient);

    /**
     * @brief sets up IClientMultiton instances to communicate with the SCPI::SCPIInterface
     * @param t_clientMultiton
     */
    void onClientIdentified(ResourceServer::IClientMultiton *t_clientMultiton);

signals:
    /**
     * @brief notifies the SCPI::SCPIInterface if a client is disconnected
     * @param t_clientMultiton
     */
    void sigClientDisconnected(ResourceServer::IClientMultiton *t_clientMultiton);

private:
    /**
     * @brief pointer to the scpi interface
     */
    SCPI::SCPIInterface *m_scpiInterface = nullptr;
    /**
      @brief  list of all Server::Client instances this server handles
      */
    QList<ClientSocket*> m_clientSockets;

    /**
     * @brief Internally used server
     */
    XiQNetServer* m_zServer = nullptr;

    /**
     * @brief network messages get decoded by the default wrapper
     */
    RMProtobufWrapper* m_defaultWrapper = nullptr;

    /**
      @note Instances of this class should only get accessed through the getInstance method.
      */
    Q_DISABLE_COPY(ServerInterface)
};
}
#endif // H2012_SERVERINTERFACE_H
