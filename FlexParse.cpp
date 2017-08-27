#include "FlexParse.h"

#include <stdio.h>

extern "C" void yyinit(void);
extern char retVal[];
extern char inpVal[];
extern keyWordCode_t kwc;

FlexParse::FlexParse()
{

}

QString FlexParse::parse(QString inp)
{
    inp += "\n";
    sprintf(inpVal,"%s",inp.toLatin1().data());
    retVal[0] = '\0';
    kwc = key_None;
    yyinit();
    if (kwc != key_None)
    {
        m_kwc = kwc;
    }
    return QString(retVal);
}
