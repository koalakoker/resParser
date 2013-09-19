#include "commandmngrclass.h"

CommandMngrClass::CommandMngrClass()
{
    m_lastCommand = 0;
    m_commandIndex = 0;
}

ParserClass* CommandMngrClass::Parser(void)
{
    return &m_parser;
}

QString CommandMngrClass::AddNewCommand(QString qsInput)
{
    m_commands.append(qsInput);
    m_commandIndex = m_lastCommand;
    m_lastCommand++;

    /* execute command */
    QString retVal;

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
    else if ((retVal = Parser()->UserDefineFunctionFormulaFromName(qsInput))!="") // Sbagliato
    {
        /* Append user defined formula if name is in qsInput */
        retVal = FormatAnswer(retVal).append("<br>");
    }
    else
    {
        hfloat result = m_parser.Parse(qsInput);
        QString ansStr;
        if (!result.isNan())
        {
            m_parser.StoreVariable("ans",result); // Store the last result
            ansStr = QString("ans=");
        }
        retVal.append(FormatAnswer(QString("%1%2<br>").arg(ansStr).arg(result.toString())));
    }
    return retVal;
}

QString CommandMngrClass::GetPreviousCommand(void)
{
    QString retVal = "";
    if ((m_commandIndex < m_commands.count()) && (m_commandIndex >= 0))
    {
        retVal = m_commands[m_commandIndex];
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
        retVal = m_commands[m_commandIndex];
    }
    return  retVal;
}

hfloat CommandMngrClass::PreviewResult(QString qsInput)
{
    return m_parser.Parse(qsInput,true);
}

QStringList CommandMngrClass::BuiltInFunctionList(void)
{
    return m_parser.builtInFunctionList();
}

QString CommandMngrClass::FormatAnswer(QString str)
{
    return QString("<font size=""4""><b>&nbsp;&nbsp;")+str+QString("</b></font>");
}
