#include "commandmngrclass.h"

#include <QFile>
#include <QDataStream>
#include "global.h"

CommandMngrClass::CommandMngrClass(QObject *parent):
    QObject(parent)
{
    m_commandIndex = 0;
    m_useFlex = true;

    connect(&m_parser,SIGNAL(ClearHistory()),this,SLOT(ClearHistorySlot()));
}

ParserClass* CommandMngrClass::Parser(void)
{
    return &m_parser;
}

QString CommandMngrClass::AddNewCommand(QString qsInput)
{
    QString retVal;
    CommandClass newCmd;
    hfloat result;

    newCmd.setInputStr(qsInput);
    if (m_useFlex)
    {
        QString parse = m_parser.m_flexParse.parse(qsInput);

        keyWordCode_t code = m_parser.m_flexParse.m_kwc;
        if (code != key_None)
        {
            retVal = m_parser.Exec(code);
        }
        else
        {
            retVal = qsInput + "<br>" + m_parser.FormatAnswer("ans=" + parse) + "<br>";
            result = parse.toDouble();
        }
    }
    else
    {
        retVal = m_parser.Exec(qsInput,result);
    }

    if (!result.isNan())
    {
        newCmd.setResult(result);
    }

    m_commands.append(newCmd);
    m_commandIndex = m_commands.count();

    return retVal;
}

QString CommandMngrClass::GetPreviousCommand(void)
{
    QString retVal = "";
    m_commandIndex--;
    if (m_commandIndex < 0)
    {
        m_commandIndex = 0;
    }
    if (m_commandIndex < m_commands.count())
    {
        retVal = m_commands[m_commandIndex].inputStr();
    }
    return  retVal;
}

QString CommandMngrClass::GetNextCommand(void)
{
    QString retVal = "";
    m_commandIndex++;

    if ((m_commandIndex < m_commands.count()) && (m_commandIndex >= 0))
    {
        retVal = m_commands[m_commandIndex].inputStr();
    }
    else
    {
        m_commandIndex = m_commands.count();
    }
    return  retVal;
}

QString CommandMngrClass::PreviewResult(QString qsInput)
{
    QString retVal;

    if (m_useFlex)
    {
        QString parse = m_parser.m_flexParse.parse(qsInput);
        if (parse != "")
        {
            retVal = QString("ans=") + parse;
        }
    }
    else
    {
        hfloat result = m_parser.Parse(qsInput,true);
        if (!result.isNan())
        {
            retVal = QString("ans=") + result.toString(m_parser.FormatOutput());
        }
    }

    return retVal;
}

QStringList CommandMngrClass::BuiltInFunctionList(void)
{
    return m_parser.builtInFunctionList();
}

QString CommandMngrClass::OutputPaneReprint(void)
{
    QString retVal;
    int i,l;
    l = m_commands.count();
    for (i = 0; i < l; i++)
    {
        QString inputStr = m_commands[i].inputStr();
        retVal.append(inputStr);
        retVal.append("<br>");
        hfloat result = m_commands[i].result();
        retVal.append(m_parser.FormatAnswer(QString("%1%2<br>").arg("ans=").arg(result.toString(m_parser.FormatOutput()))));
        if (i < (l-1))
        {
            retVal.append("<br>");
        }
    }
    return retVal;
}

void CommandMngrClass::Save(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    // Store commands
    int l = m_commands.count();
    out << (qint32)(l);
    int i;
    for (i = 0; i < l; i++)
    {
        out << m_commands[i].inputStr();
        out << m_commands[i].result().toString(FIXED_MAX_PRECISION);
    }

    // Store Parser state
    m_parser.Save(out);;
}

void CommandMngrClass::Load(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    // Load commands
    int i,l;
    in >> l;
    m_commands.clear();
    for (i = 0; i < l; i++)
    {
        CommandClass newCmd;
        QString inputStr;
        QString resultStr;
        in >> inputStr;
        in >> resultStr;
        newCmd.setInputStr(inputStr);
        hfloat result(resultStr);
        newCmd.setResult(result);
        m_commands.append(newCmd);
    }
    m_commandIndex = l;

    // Load Parser state
    m_parser.Load(in);
}

TableInfo CommandMngrClass::VariableInfo(void)
{
    TableInfo infoTable;
    infoTable.setColumn(2);
    int i;
    for (i = 0; i < m_parser.VariableCreated(); i++)
    {
        Variable* var = m_parser.GetVariableAtIndex(i);
        TableInfoElement tab;
        tab.m_value.append(var->Name());
        tab.m_value.append(var->Value().toString(m_parser.FormatOutput()));
        infoTable.append(tab);
    }
    return infoTable;
}

TableInfo CommandMngrClass::BuiltInFunctionInfo(void)
{
    TableInfo infoTable;
    infoTable.setColumn(2);
    QStringList list = m_parser.builtInFunctionList();
    int i;
    for (i = 0; i < list.count(); i++)
    {
        TableInfoElement tab;
        tab.m_value.append(list.at(i));
        tab.m_value.append("");
        infoTable.append(tab);
    }
    return infoTable;
}

TableInfo CommandMngrClass::UserFunctionInfo(void)
{
    return m_parser.UserDefinedFunctionsInfo();
}

void CommandMngrClass::ClearHistorySlot(void)
{
    m_commands.clear();
    emit ClearHistorySignal();
}
