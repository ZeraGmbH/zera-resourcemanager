#include "delegate.h"

namespace SCPI
{
  Delegate::Delegate() : cSCPIObject()
  {
  }

  Delegate::Delegate(const QString &name, quint8 type) : cSCPIObject(name, type)
  {
  }

  bool Delegate::executeSCPI(const QString &sInput, QString &sOutput)
  {
    Q_UNUSED(sInput)
    Q_UNUSED(sOutput)
    //error
    return false;
  }

}
