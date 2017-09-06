#include "FlexParse.h"
#include "calc.h"

#include <stdio.h>

extern "C" void yyExec(void);
extern "C" void flexDeleteFunc(char* sym);
extern char retVal[];
extern char inpVal[];
extern keyWordCode_t kwc;
extern char* symbol;
extern int newAssignVar;
extern int newAssignFunc;
extern struct symlist *symListRet;

FlexParse::FlexParse()
{

}

void FlexParse::deleteFunc(QString name)
{
    flexDeleteFunc(name.toLatin1().data());
}

QString FlexParse::parse(QString inp)
{
    inp += "\n";
    sprintf(inpVal,"%s",inp.toLatin1().data());
    retVal[0] = '\0';
    kwc = key_None;
    symbol = 0;
    newAssignVar = 0;
    newAssignFunc = 0;
    symListRet = 0;

    yyExec();

    m_kwc = kwc;

    m_newAssignVar = (newAssignVar == 1);
    m_newAssignFunc = (newAssignFunc == 1);

    m_args.clear();
    while (symListRet != 0)
    {
        m_args.append(symListRet->sym->name);
        symListRet = symListRet->next;
    }

    m_symbol = "";
    if (symbol)
    {
        m_symbol = QString(symbol);
    }

    return QString(retVal);
}
