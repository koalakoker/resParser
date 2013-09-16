#include "commandmngrclass.h"

CommandMngrClass::CommandMngrClass()
{
    lastCommand = 0;
    commandIndex = 0;
}

QString CommandMngrClass::AddNewCommand(QString qsInput)
{
    commands.append(qsInput);
    commandIndex = lastCommand;
    lastCommand++;

    /* execute command */
    QString retVal;
    if ((qsInput.contains("list"))||(qsInput.contains("ls")))
    {
        int i;
        for (i = 0; i < parser.VariableCreated(); i++)
        {
            Variable* var = parser.GetVariableAtIndex(i);
            retVal.append(var->ToString());
            retVal.append("<br>");
        }
    }
    else if (qsInput.contains("clear"))
    {
        parser.Clear();
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
    else
    {
        hfloat result = parser.Parse(qsInput);
        parser.StoreVariable("ans",result); // Store the last result
        retVal.append(formatAnswer(QString("ans=%1<br>").arg(result.toString())));
    }
    return retVal;
}

QString CommandMngrClass::GetPreviousCommand(void)
{
    QString retVal = "";
    if ((commandIndex < commands.count()) && (commandIndex >= 0))
    {
        retVal = commands[commandIndex];
        if (commandIndex > 0)
        {
            commandIndex--;
        }
    }
    return  retVal;
}

QString CommandMngrClass::GetNextCommand(void)
{
    QString retVal = "";
    if (commandIndex < lastCommand - 1)
    {
        commandIndex++;
    }
    if ((commandIndex < commands.count()) && (commandIndex >= 0))
    {
        retVal = commands[commandIndex];
    }
    return  retVal;
}

hfloat CommandMngrClass::PreviewResult(QString qsInput)
{
    return parser.Parse(qsInput);
}

QStringList CommandMngrClass::builtInFunctionList(void)
{
    return parser.builtInFunctionList();
}

QString CommandMngrClass::formatAnswer(QString str)
{
    return QString("<font size=""4""><b>&nbsp;&nbsp;")+str+QString("</b></font>");
}
