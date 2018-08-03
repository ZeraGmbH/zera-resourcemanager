#include "rmprotobufwrapper.h"

#include <netmessages.pb.h>
#include <QDebug>

RMProtobufWrapper::RMProtobufWrapper()
{
}


ProtobufPointer RMProtobufWrapper::byteArrayToProtobuf(QByteArray t_data)
{
  ProtobufPointer proto {new ProtobufMessage::NetMessage()};
  if(!proto->ParseFromArray(t_data, t_data.size()))
  {
    qCritical() << "Error parsing protobuf:\n" << t_data.toBase64();
    Q_ASSERT(false);
  }
  return proto;
}

QByteArray RMProtobufWrapper::protobufToByteArray(const google::protobuf::Message &t_protobufMessage)
{
  return QByteArray(t_protobufMessage.SerializeAsString().c_str(), t_protobufMessage.ByteSize());
}
