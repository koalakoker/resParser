#include "builtinfunction.h"

builtinFunction::builtinFunction(QString name,hfloat (*ptr1a)(hfloat a))
{
    this->m_name = name;
    this->m_ptr1a = ptr1a;
}

builtinFunction::builtinFunction(QString name,hfloat (*ptr2a)(hfloat a,hfloat b))
{
    this->m_name = name;
    this->m_ptr2a = ptr2a;
}

QString builtinFunction::name(void)
{
    return this->m_name;
}

hfloat builtinFunction::exec(void)
{
    hfloat retVal;
    switch (arg.count())
    {
    case 0:
    {
        if (m_ptr0a)
        {
            retVal = (*m_ptr0a)();
        }
        break;
    }
    case 1:
    {
        if (m_ptr1a)
        {
            retVal = (*m_ptr1a)(arg[0]);
        }
        break;
    }
    case 2:
    {
        if (m_ptr2a)
        {
            retVal = (*m_ptr2a)(arg[0],arg[1]);
        }
        break;
    }
    }
    return retVal;
}

void builtinFunction::clearArgs(void)
{
    arg.clear();
}

void builtinFunction::addArg(hfloat arg)
{
    this->arg.append(arg);
}
