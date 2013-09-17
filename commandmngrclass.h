#ifndef COMMANDMNGRCLASS_H
#define COMMANDMNGRCLASS_H

#include "QStringList"
#include "parserclass.h"

class CommandMngrClass
{
private:
    ParserClass m_parser;
    QStringList m_commands;
    int m_lastCommand;
    int m_commandIndex;

    QString FormatAnswer(QString str);
public:
    CommandMngrClass();
    QString AddNewCommand(QString qsInput);
    QString GetPreviousCommand(void);
    QString GetNextCommand(void);
    hfloat PreviewResult(QString qsInput);
    QStringList BuiltInFunctionList(void);
    ParserClass* Parser(void);
};

#endif // COMMANDMNGRCLASS_H
