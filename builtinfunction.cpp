#include "builtinfunction.h"

builtinFunction::builtinFunction(QString name,hfloat (*ptr1a)(hfloat a))
{
    this->m_name = name;
    this->m_ptr1a = ptr1a;
}

QString builtinFunction::name(void)
{
    return this->m_name;
}

hfloat builtinFunction::exec(hfloat* arg, int argc)
{
    hfloat retVal;
    if ((argc==1)&&(m_ptr1a))
    {
        retVal = (*m_ptr1a)(arg[0]);
    }
    return retVal;
}
