#include "commandmngrclass.h"

#include <QFile>
#include <QDataStream>

#define FIXED_MAX_PRECISION "%.50Rf"
#define FIXED "%.%1Rf"
#define SCIENTIFIC "%.%1Re"
#define AUTO "%.%1Rg"
#define HEXADECIMAL "%lX"

CommandMngrClass::CommandMngrClass()
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
    else if (qsInput.contains("clear"))
    {
        m_parser.Clear();
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
    int l = m_commands.count();
    out << (qint32)(l);
    int i;
    for (i = 0; i < l; i++)
    {
        out << m_commands[i].inputStr();
        out << m_commands[i].result().toString(FIXED_MAX_PRECISION);
    }
}

void CommandMngrClass::Load(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    qint32 qil;
    in >> qil;
    int i;
    m_commands.clear();
    for (i = 0; i < (int)(qil); i++)
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
}
