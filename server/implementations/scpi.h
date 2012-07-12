#ifndef H2012_SCPIIMPLEMENTATION_H
#define H2012_SCPIIMPLEMENTATION_H

#include <QObject>
#include <QMap>

namespace Resource
{
  class ResourceObject;
  class Detail;
}
namespace Server
{
  class Client;

  namespace Implementations
  {
    /**
      @b Default SCPI implementation
      */
    class Scpi : public QObject
    {
      Q_OBJECT
    public:
      /**
        @b default constructor
        */
      explicit Scpi(QObject *parent = 0);

    public slots:

    private:
      /**
        @brief Holds a list of scpi clients that correspond the Server::Client objects this class created.
        @warning Not implemented yet
        */
      QMap<int, QVariant*> scpiClients;
    };
  }
}
#endif // H2012_SCPIIMPLEMENTATION_H
