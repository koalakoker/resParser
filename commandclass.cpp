#include "commandclass.h"

CommandClass::CommandClass()
{
}

CommandClass::CommandClass(const CommandClass &org)
{
    m_inputStr = org.m_inputStr;
    m_result = org.m_result;
}

void CommandClass::setInputStr(QString inputStr)
{
    m_inputStr = inputStr;
}

void CommandClass::setResult(hfloat result)
{
    m_result = result;
}

QString CommandClass::inputStr(void)
{
    return m_inputStr;
}

hfloat CommandClass::result(void)
{
    return m_result;
}
