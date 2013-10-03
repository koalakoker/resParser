#include "commandmngrclass.h"

#include <QFile>
#include <QDataStream>
#include "global.h"

#define FIXED "%.%1Rf"
#define SCIENTIFIC "%.%1Re"
#define AUTO "%.%1Rg"
#define HEXADECIMAL "%lX"

CommandMngrClass::CommandMngrClass(QObject *parent):
    QObject(parent)
{
    m_lastCommand = 0;
    m_commandIndex = 0;
    m_formatOutput = Auto;
    m_precision = 20;
}

ParserClass* CommandMngrClass::Parser(void)
{
    return &m_parser;
}

formatOutput_t CommandMngrClass::Format(void)
{
    return m_formatOutput;
}

void CommandMngrClass::SetFormat(formatOutput_t format)
{
    m_formatOutput = format;
}

int CommandMngrClass::Precision(void)
{
    return m_precision;
}

void CommandMngrClass::SetPrecision(int precision)
{
    m_precision = precision;
}

QString CommandMngrClass::AddNewCommand(QString qsInput)
{
    QString retVal,tmpVal;
    CommandClass newCmd;
    newCmd.setInputStr(qsInput);

    retVal = qsInput + QString("<br>");

    /* execute command */
    if ((qsInput.contains("list"))||(qsInput.contains("ls")))
    {
        int i;
        for (i = 0; i < m_parser.VariableCreated(); i++)
        {
            Variable* var = m_parser.GetVariableAtIndex(i);
            retVal.append(var->ToString());
            retVal.append("<br>");
        }
    }
    else if (qsInput.toLower().contains("clear"))
    {
        // Remove spaces
        qsInput.replace(" ","");
        if (m_parser.IsVariableName(qsInput.remove("clear")))
        {
            m_parser.RemoveVariable(qsInput);
        }
        else if (m_parser.IsUserDefinedFunctionName(qsInput))
        {
            m_parser.RemoveUserDefinedFunction(qsInput);
        }
        else if (qsInput.toLower() == "clear" )
        {
            m_parser.Clear();
        }
    }
    else if (qsInput == "E12")
    {
        retVal.append(Resistor::E12ValuesToString());
        retVal.append("<br>");
    }
    else if (qsInput == "E24")
    {
        retVal.append(Resistor::E24ValuesToString());
        retVal.append("<br>");
    }
    else if (qsInput.toLower() == "usage")
    {
        retVal.append("<br>");
        retVal.append("Using built-in function <b>function_name(arg,[arg])</b>.<br>");
        retVal.append("Variable assignment: <b>var_name = expression</b>.<br>");
        retVal.append("Show variables <b>list</b> or <b>ls</b>.<br>");
        retVal.append("<b>clear</b> to delete all varaibles.<br>");
        retVal.append("<b>E12</b> to show all E12 resitor values.<br>");
        retVal.append("<b>E24</b> to show all E24 resitor values.<br>");
        retVal.append("<b>->E12</b> to round to nearest E12 resitor values.<br>");
        retVal.append("<b>:</b> parallel operator between resistors.<br>");
    }
    else if ((tmpVal = Parser()->UserDefineFunctionFormulaFromName(qsInput))!="")
    {
        /* Append user defined formula if name is in qsInput */
        retVal.append(FormatAnswer(tmpVal).append("<br>"));
    }
    else
    {
        hfloat result = m_parser.Parse(qsInput);
        QString ansStr;
        if (!result.isNan())
        {
            m_parser.StoreVariable("ans",result); // Store the last result
            ansStr = QString("ans=");
            newCmd.setResult(result);
        }
        retVal.append(FormatAnswer(QString("%1%2<br>").arg(ansStr).arg(result.toString(FormatOutput()))));
    }

    m_commands.append(newCmd);
    m_commandIndex = m_lastCommand;
    m_lastCommand++;

    return retVal;
}

QString CommandMngrClass::GetPreviousCommand(void)
{
    QString retVal = "";
    if ((m_commandIndex < m_commands.count()) && (m_commandIndex >= 0))
    {
        retVal = m_commands[m_commandIndex].inputStr();
        if (m_commandIndex > 0)
        {
            m_commandIndex--;
        }
    }
    return  retVal;
}

QString CommandMngrClass::GetNextCommand(void)
{
    QString retVal = "";
    if (m_commandIndex < m_lastCommand - 1)
    {
        m_commandIndex++;
    }
    if ((m_commandIndex < m_commands.count()) && (m_commandIndex >= 0))
    {
        retVal = m_commands[m_commandIndex].inputStr();
    }
    return  retVal;
}

QString CommandMngrClass::PreviewResult(QString qsInput)
{
    QString retVal;
    hfloat result = m_parser.Parse(qsInput,true);
    if (!result.isNan())
    {
        retVal = QString("ans=") + result.toString(FormatOutput());
    }
    return retVal;
}

QStringList CommandMngrClass::BuiltInFunctionList(void)
{
    return m_parser.builtInFunctionList();
}

QString CommandMngrClass::FormatAnswer(QString str)
{
    return QString("<font size=""4""><b>&nbsp;&nbsp;")+str+QString("</b></font>");
}

QString CommandMngrClass::FormatOutput(void)
{
    QString retVal;
    switch (m_formatOutput)
    {
    case Hexadecimal:
    {
        retVal = QString(HEXADECIMAL);
    }
        break;
    case Fixed:
    {
        retVal = QString(FIXED).arg(m_precision);
    }
        break;
    case Scientific:
    {
        retVal = QString(SCIENTIFIC).arg(m_precision);
    }
        break;
    case Auto:
    default:
    {
        retVal = QString(AUTO).arg(m_precision);
    }
        break;
    }
    return retVal;
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
        retVal.append(FormatAnswer(QString("%1%2<br>").arg("ans=").arg(result.toString(FormatOutput()))));
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
    m_commandIndex = l - 1;
    m_lastCommand = l;

    // Load Parser state
    m_parser.Load(in);
}

QList<TableInfoElement> CommandMngrClass::VariableInfo(void)
{
    QList<TableInfoElement> infoTable;
    int i;
    for (i = 0; i < m_parser.VariableCreated(); i++)
    {
        Variable* var = m_parser.GetVariableAtIndex(i);
        TableInfoElement tab;
        tab.m_name = var->Name();
        tab.m_value = var->Value().toString(FormatOutput());
        infoTable.append(tab);
    }
    return infoTable;
}

QList<TableInfoElement> CommandMngrClass::BuiltInFunctionInfo(void)
{
    QList<TableInfoElement> infoTable;
    QStringList list = m_parser.builtInFunctionList();
    int i;
    for (i = 0; i < list.count(); i++)
    {
        TableInfoElement tab;
        tab.m_name = list.at(i);
        tab.m_value = "";
        infoTable.append(tab);
    }
    return infoTable;
}

QList<TableInfoElement> CommandMngrClass::UserFunctionInfo(void)
{
    return m_parser.UserDefinedFunctionsInfo();
}
