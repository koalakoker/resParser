#ifndef COMMANDMNGRCLASS_H
#define COMMANDMNGRCLASS_H

#include "QStringList"
#include "parserclass.h"
#include "commandclass.h"
#include "tableinfoelement.h"

class CommandMngrClass : public QObject
{
    Q_OBJECT
public:
    ParserClass m_parser;

private:
    QList<CommandClass> m_commands;
    int m_commandIndex;
    bool m_useFlex;

public:
    explicit CommandMngrClass(QObject *parent = 0);

    ParserClass* Parser(void);

    QString AddNewCommand(QString qsInput);
    QString GetPreviousCommand(void);
    QString GetNextCommand(void);

    QString PreviewResult(QString qsInput);

    TableInfo VariableInfo(void);
    TableInfo BuiltInFunctionInfo(void);
    TableInfo UserFunctionInfo(void);
    QStringList BuiltInFunctionList(void);
    QString OutputPaneReprint(void);

    void Save(QString fileName);
    void Load(QString fileName);

signals:
    void variablesUpdate(void);
    void ClearHistorySignal(void);

public slots:
    void ClearHistorySlot(void);
};

#endif // COMMANDMNGRCLASS_H
