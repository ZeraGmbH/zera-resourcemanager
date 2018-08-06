#ifndef PROTOBUFWRAPPER_H
#define PROTOBUFWRAPPER_H

#include <xiqnetwrapper.h>

class RMProtobufWrapper : public XiQNetWrapper
{
public:
  RMProtobufWrapper();

  std::shared_ptr<google::protobuf::Message> byteArrayToProtobuf(QByteArray t_data) override;

  QByteArray protobufToByteArray(const google::protobuf::Message &t_protobufMessage) override;
};

#endif // PROTOBUFWRAPPER_H
