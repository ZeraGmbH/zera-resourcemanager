#ifndef PROTOBUFWRAPPER_H
#define PROTOBUFWRAPPER_H

#include <xiqnetwrapper.h>

class RMProtobufWrapper : public XiQNetWrapper
{
public:
  RMProtobufWrapper();

  google::protobuf::Message *byteArrayToProtobuf(QByteArray t_data) override;

  QByteArray protobufToByteArray(google::protobuf::Message *t_protobufMessage) override;
};

#endif // PROTOBUFWRAPPER_H
