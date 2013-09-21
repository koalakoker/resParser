#ifndef COMMANDMNGRCLASS_H
#define COMMANDMNGRCLASS_H

#include "QStringList"
#include "parserclass.h"

typedef enum {Fixed, Scientific, Auto} formatOutput_t;

class CommandMngrClass
{
private:
    ParserClass m_parser;
    QStringList m_commands;
    int m_lastCommand;
    int m_commandIndex;
    formatOutput_t m_formatOutput;
    int m_precision;

    QString FormatAnswer(QString str);
public:
    CommandMngrClass();
    QString AddNewCommand(QString qsInput);
    QString GetPreviousCommand(void);
    QString GetNextCommand(void);
    QString PreviewResult(QString qsInput);
    QStringList BuiltInFunctionList(void);
    ParserClass* Parser(void);
    char* FormatOutput(void);
    formatOutput_t Format(void);
    void SetFormat(formatOutput_t format);
    int Precision(void);
    void SetPrecision(int precision);
};

#endif // COMMANDMNGRCLASS_H
