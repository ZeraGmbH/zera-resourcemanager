#ifndef DELEGATE_H
#define DELEGATE_H

#include <scpiobject.h>

/**
  @brief This will represent commands in the SCPI tree

  But it will not implement any logic
  */
class Delegate : public cSCPIObject
{
public:
  Delegate();
  /**
    @b Reimplemented constructor
    */
  Delegate(const QString& name, quint8 type);
  /**
    @b Leave this up to the SCPIInterface
    */
  bool executeSCPI(const QString &sInput, QString &sOutput);
};

#endif // DELEGATE_H
