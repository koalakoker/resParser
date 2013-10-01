#ifndef COMMANDMNGRCLASS_H
#define COMMANDMNGRCLASS_H

#include "QStringList"
#include "parserclass.h"
#include "commandclass.h"
#include "tableinfoelement.h"

typedef enum {Fixed, Scientific, Auto, Hexadecimal} formatOutput_t;

class CommandMngrClass : public QObject
{
    Q_OBJECT
private:
    ParserClass m_parser;
    QList<CommandClass> m_commands;
    int m_lastCommand;
    int m_commandIndex;
    formatOutput_t m_formatOutput;
    int m_precision;

    QString FormatAnswer(QString str);
public:
    explicit CommandMngrClass(QObject *parent = 0);

    ParserClass* Parser(void);

    QString AddNewCommand(QString qsInput);
    QString GetPreviousCommand(void);
    QString GetNextCommand(void);

    QString PreviewResult(QString qsInput);

    QList<TableInfoElement> VariableInfo(void);
    QList<TableInfoElement> BuiltInFunctionInfo(void);
    QList<TableInfoElement> UserFunctionInfo(void);
    QStringList BuiltInFunctionList(void);
    QString OutputPaneReprint(void);

    void Save(QString fileName);
    void Load(QString fileName);

    formatOutput_t Format(void); // Returns the current selected format
    int Precision(void); // Returns the current select precision in digit
    void SetFormat(formatOutput_t format); // Sets the format
    void SetPrecision(int precision); // Sets the precision in digit
    QString FormatOutput(void); // Return the format string to pass to hfloat::toString()
signals:
    void variablesUpdate(void);
};

#endif // COMMANDMNGRCLASS_H
