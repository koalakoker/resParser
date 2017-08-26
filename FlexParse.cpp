#include "FlexParse.h"

#include <stdio.h>

extern "C" void yyinit(void);
extern char retVal[];
extern char inpVal[];

FlexParse::FlexParse()
{

}

QString FlexParse::parse(QString inp)
{
    inp += "\n";
    sprintf(inpVal,"%s",inp.toLatin1().data());
    retVal[0] = '\0';
    yyinit();
    return QString(retVal);
}
