#ifndef COMMANDCLASS_H
#define COMMANDCLASS_H

#include <QString>
#include "hfloat.h"

class CommandClass
{
public:
    CommandClass();
    CommandClass(const CommandClass& org);

    void setInputStr(QString inputStr);
    void setResult(hfloat result);
    QString inputStr(void);
    hfloat result(void);

private:
    QString m_inputStr;
    hfloat m_result;
};

#endif // COMMANDCLASS_H
