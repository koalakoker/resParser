#include "userdefinedfunctions.h"

userdefinedFunctions::userdefinedFunctions()
{
}

void userdefinedFunctions::setName(QString name)
{
    this->m_name = name;
}

void userdefinedFunctions::setFunctionStr(QString str)
{
    this->m_functionStr = str;
}

void userdefinedFunctions::setArgs(QStringList args)
{
    this->m_args = args;
}

QString userdefinedFunctions::Name(void)
{
    return m_name;
}

QString userdefinedFunctions::functionSrt(void)
{
    return m_functionStr;
}

QStringList userdefinedFunctions::args(void)
{
    return m_args;
}

QString userdefinedFunctions::toString(void)
{
    QString retVal = m_name + QString("(");
    int i;
    for (i = 0; i < m_args.count(); i++)
    {
        retVal.append(m_args[i]);
        if (i < m_args.count()-1)
        {
            retVal.append(",");
        }
    }
    retVal.append(")=");
    retVal.append(m_functionStr);
    return retVal;
}
