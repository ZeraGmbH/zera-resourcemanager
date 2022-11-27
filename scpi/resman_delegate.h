#ifndef DELEGATE_H
#define DELEGATE_H

#include <scpiobject.h>

namespace SCPI
{
/**
  @brief This will represent commands in the SCPI tree

  But it will not implement any logic
  */
class Delegate : public cSCPIObject
{
public:
    Delegate();
    /**
    @brief Reimplemented constructor
    */
    Delegate(const QString& t_name, quint8 t_type);
    /**
    @brief Leave this up to the SCPI::SCPIInterface
    */
    bool executeSCPI(const QString &t_input, QString &t_output) override;
};
}
#endif // DELEGATE_H
