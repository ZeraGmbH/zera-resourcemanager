#include "delegate.h"

Delegate::Delegate() : cSCPIObject()
{
}

Delegate::Delegate(const QString &name, quint8 type) : cSCPIObject(name, type)
{
}

bool Delegate::executeSCPI(const QString &sInput, QString &sOutput)
{
  //error
  return false;
}
