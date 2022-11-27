#include "rmprotobufwrapper.h"

#include <netmessages.pb.h>
#include <QDebug>

RMProtobufWrapper::RMProtobufWrapper()
{
}


std::shared_ptr<google::protobuf::Message> RMProtobufWrapper::byteArrayToProtobuf(QByteArray t_data)
{
    std::shared_ptr<google::protobuf::Message> proto {new ProtobufMessage::NetMessage()};
    if(!proto->ParseFromArray(t_data, t_data.size()))
    {
        qCritical() << "Error parsing protobuf:\n" << t_data.toBase64();
        Q_ASSERT(false);
    }
    return proto;
}
