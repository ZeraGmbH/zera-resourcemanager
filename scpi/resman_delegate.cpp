#include "resman_delegate.h"

namespace SCPI
{
Delegate::Delegate(const QString &t_name, quint8 t_type) : cSCPIObject(t_name, t_type)
{
}

bool Delegate::executeSCPI(const QString &t_input, QString &t_output)
{
    Q_UNUSED(t_input)
    Q_UNUSED(t_output)
    //error
    return false;
}

}
