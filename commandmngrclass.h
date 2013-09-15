#ifndef COMMANDMNGRCLASS_H
#define COMMANDMNGRCLASS_H

#include "QStringList"
#include "parserclass.h"

class CommandMngrClass
{
private:
    ParserClass parser;
    QStringList commands;
    int lastCommand;
    int commandIndex;
public:
    CommandMngrClass();
    QString AddNewCommand(QString qsInput);
    QString GetPreviousCommand(void);
    QString GetNextCommand(void);
    hfloat PreviewResult(QString qsInput);
    QStringList builtInFunctionList(void);
};

#endif // COMMANDMNGRCLASS_H
